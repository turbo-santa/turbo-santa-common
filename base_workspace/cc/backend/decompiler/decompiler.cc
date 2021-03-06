#include "cc/backend/decompiler/decompiler.h"

#include <iomanip>
#include <sstream>

#include "glog/logging.h"

namespace backend {
namespace decompiler {

using std::endl;
using std::hex;
using std::left;
using std::map;
using std::ostream;
using std::resetiosflags;
using std::setfill;
using std::setw;
using std::string;
using std::stringstream;

namespace {
bool GetJumpAddress(const Instruction& instruction, uint16_t* address) {
  if (instruction.opcode == Opcode::RET 
      || instruction.opcode == Opcode::RETI
      || instruction.instruction == 0xe9) {
    // Cannot decode these opcode's jumps at compile time.
    return false;
  }

  if (instruction.arg1.type == ArgumentType::VALUE) {
    if (instruction.opcode == Opcode::JR) {
      *address += static_cast<int8_t>(instruction.arg1.value.val);
    } else {
      *address = instruction.arg1.value.val;
    }
    return true;
  } else if (instruction.arg2.type == ArgumentType::VALUE) {
    if (instruction.opcode == Opcode::JR) {
      *address += static_cast<int8_t>(instruction.arg2.value.val);
    } else {
      *address = instruction.arg2.value.val;
    }
    return true;
  } else {
    LOG(FATAL) << "Error extracting jump address.";
  }
}

bool IsJumpConditionalOrCall(const Instruction& instruction) {
  if (instruction.opcode == Opcode::CALL) {
    return true;
  } else if (instruction.opcode == Opcode::RET
             && instruction.arg1.type != ArgumentType::EMPTY) {
    return true;
  } else if (instruction.arg2.type == ArgumentType::EMPTY) {
    return false;
  } else {
    return true;
  }
}

string Hex(uint16_t address) {
  stringstream stream;
  stream << "0x" << hex << setfill('0') << setw(4) << address;
  return stream.str();
}

string CreateLabel(uint16_t address) {
  stringstream stream;
  stream << "Addr_" << Hex(address);
  return stream.str();
}

string ArgumentToString(const map<Register, string>& register_map, 
                        const Parameter& param) {
  stringstream stream;
  switch (param.type) {
    case ArgumentType::EMPTY:
      return "";
    case ArgumentType::REGISTER:
      if (param.is_pointer) {
        stream << "(" << register_map.at(param.value.reg) << ")";
      } else {
        stream << register_map.at(param.value.reg);
      }
      return stream.str();
    case ArgumentType::VALUE:
      if (param.is_pointer) {
        stream << "(" << Hex(param.value.val) << ")";
      } else {
        stream << Hex(param.value.val);
      }
      return stream.str();
  }
}

string JumpToString(const map<Opcode, string>& opcode_map,
                    const map<Register, string>& register_map,
                    const Instruction& instruction,
                    uint16_t address) {
  address += 2;
  stringstream stream;
  stream << opcode_map.at(instruction.opcode);
  if (instruction.arg1.type == ArgumentType::REGISTER) {
    stream << " " << ArgumentToString(register_map, instruction.arg1);
  } else if (GetJumpAddress(instruction, &address)) {
    stream << " " << CreateLabel(address);
  }
  if (instruction.arg2.type != ArgumentType::EMPTY
      && GetJumpAddress(instruction, &address)) {
    stream << ", " << CreateLabel(address);
  }
  return stream.str();
}

string InstructionToString(const map<Opcode, string>& opcode_map,
                           const map<Register, string>& register_map,
                           const Instruction& instruction,
                           uint16_t address) {
  if (instruction.is_jump) {
    return JumpToString(opcode_map, register_map, instruction, address);
  }

  stringstream stream;
  stream << opcode_map.at(instruction.opcode);
  if (instruction.arg1.type != ArgumentType::EMPTY) {
    stream << " " << ArgumentToString(register_map, instruction.arg1);
  }
  if (instruction.arg2.type != ArgumentType::EMPTY) {
    stream << ", " << ArgumentToString(register_map, instruction.arg2);
  }
  return stream.str();
}
} // namespace

void Decompiler::Decompile() {
  LOG(INFO) << "Decompiling...";
  if (rom_type_ != 0x00) {
    LOG(WARNING) << "ROM has an MBC which is not currently supported.";
  }
  while (!code_paths_.empty()) {
    uint16_t next_address = code_paths_.top();
    code_paths_.pop();
    if (!DecompileInstructionAt(next_address)) {
      LOG(ERROR) << "Was not able to finish decompiling.";
      return;
    }
  }
  LOG(INFO) << "Decompilation complete.";
}

bool Decompiler::LookUp(uint16_t address, Instruction* instruction) {
  auto iter = address_opcode_map_.find(address);
  if (iter != address_opcode_map_.end()) {
    *instruction = iter->second;
    return true;
  } else {
    return false;
  }
}

void Decompiler::PrintToStream(ostream* out_stream) {
  for (auto iter = address_opcode_map_.begin(); iter != address_opcode_map_.end(); iter++) {
    *out_stream << PrintInstruction(iter->first);
  }
}

bool Decompiler::DecompileInstructionAt(uint16_t address) {
  if (address_opcode_map_.find(address) != address_opcode_map_.end()) {
    // We have already decompiled this part.
    return true;
  }
  if (allocation_map_.find(address) != allocation_map_.end()) {
    auto iter = address_opcode_map_.end();
    uint16_t existing_address = address;
    while (iter == address_opcode_map_.end()) {
      existing_address--;
      iter = address_opcode_map_.find(existing_address);
    }
    LOG(ERROR) << "Attempted to decompile instruction at 0x" 
        << std::hex << address << ", but decompiled opcode starting at "
        << std::hex << existing_address << " occupies this address.";
    return false;
  }
  Instruction instruction;
  if (!rom_reader_.Read(address, &instruction)) {
    return false;
  }
  AddInstruction(address, instruction);
  if (instruction.is_jump) {
    uint16_t jump_address = address + instruction.instruction_width_bytes;
    if (GetJumpAddress(instruction, &jump_address) 
        && rom_min_address_ <= jump_address 
        && jump_address <= rom_max_address_) {
      code_paths_.push(jump_address);
      addresses_jumped_to_.insert(jump_address);
    }
    if (!IsJumpConditionalOrCall(instruction)) {
      // Jump is not conditional or a call so the address after this instruction 
      // may not belong to any valid code path.
      return true;
    }
  }
  uint16_t next_address = address + instruction.instruction_width_bytes;
  if (next_address <= rom_max_address_) {
    code_paths_.push(next_address);
  }
  return true;
}

void Decompiler::AddInstruction(uint16_t address, Instruction instruction) {
  address_opcode_map_[address] = instruction;
  for (int i = 0; i < instruction.instruction_width_bytes; i++) {
    allocation_map_[address + i] = &instruction;
  }
}

string Decompiler::PrintInstruction(uint16_t address) {
  stringstream stream;
  if (addresses_jumped_to_.find(address) != addresses_jumped_to_.end()) {
    stream << CreateLabel(address) << ":" << endl;
  }
  stream << "  " 
      << setfill(' ') << left << setw(22)
      << InstructionToString(
          opcode_map_, 
          register_map_, 
          address_opcode_map_[address],
          address)
      << resetiosflags(std::ios_base::basefield)
      << "; " << Hex(address) << endl;
  return stream.str();
}

} // namespace decompiler
} // namespace backend
