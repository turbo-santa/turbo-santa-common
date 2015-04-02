#include "backend/decompiler/rom_reader.h"

#include "submodules/glog/src/glog/logging.h"

namespace back_end {
namespace decompiler {

uint16_t OpcodeValue(uint8_t first, uint8_t second) {
  if (first == 0xcb && (second & 0b11000000) > 0) {
    return (first << 8) | (second & 0b11000111);
  } else if (first == 0xcb) {
    return (first << 8) | second;
  } else if (second == 0x10 && first == 0x00) {
    return (second << 8) | first;
  } else {
    return first;
  }
}

const RawInstructionBase& ROMReader::raw_instruction(uint16_t address, ValueWidth width) {
  switch (width) {
    case ValueWidth::BIT_0:
      LOG(FATAL) << "Cannot have instruction of length zero.";
    case ValueWidth::BIT_8:
      raw_instruction_8bit_.set_ptr(rom_->data() + address);
      return raw_instruction_8bit_;
    case ValueWidth::BIT_16:
      if (address + 2 >= rom_->size()) {
        LOG(FATAL) << "Instruction should be 16 bits wide but only one byte remains.";
      }
      raw_instruction_16bit_.set_ptr(rom_->data() + address);
      return raw_instruction_16bit_;
    case ValueWidth::BIT_24:
      if (address + 3 >= rom_->size()) {
        LOG(FATAL) << "Instruction should be 24 bits wide but fewer remain.";
      }
      raw_instruction_24bit_.set_ptr(rom_->data() + address);
      return raw_instruction_24bit_;
  }
}

Instruction ROMReader::Read(uint16_t address) {
  uint16_t opcode;
  if (address >= rom_->size()) {
    LOG(FATAL) << "Attempted illegal access of ROM at: 0x" << std::hex << address;
  } else if (address + 1 == rom_->size()) {
    opcode = rom_->at(address);
  } else {
    opcode = OpcodeValue(rom_->at(address), rom_->at(address + 1));
  }

  auto iter = instruction_map_.find(opcode);
  if (iter == instruction_map_.end()) {
    LOG(FATAL) << "Opcode value, 0x" << std::hex << opcode << ", does not exist.";
  }
  const InstructionFactory& factory = iter->second;
  return factory.Build(raw_instruction(address, factory.total_width()));
}

} // namespace decompiler
} // namespace decompiler
