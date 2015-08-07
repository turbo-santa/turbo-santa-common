#include "cc/backend/opcode_executor/opcode_parser.h"

#include <fstream>
#include <sstream>
#include "cc/backend/decompiler/decompiler.h"
#include "cc/backend/decompiler/decompiler_factory.h"
#include "cc/backend/memory/memory_layout.h"
#include "submodules/glog/src/glog/logging.h"

namespace backend {
namespace opcode_executor {

using decompiler::Decompiler;
using decompiler::DecompilerFactory;
using decompiler::Instruction;
using std::ofstream;

namespace {
uint16_t GetArg1(const Instruction& instruction) {
  return instruction.arg1.value.val;
}

uint16_t GetArg2(const Instruction& instruction) {
  return instruction.arg2.value.val;
}

bool GetJumpAddress(const Instruction& instruction, 
                    uint16_t current_address,
                    uint16_t return_value,
                    uint16_t hl_value,
                    uint16_t* address) {
  if (!instruction.is_jump) {
    return false;
  }

  switch (instruction.instruction) {
    // JP NN
    case 0xc3:
      *address = GetArg1(instruction);
      break;
    // JP CC, NN
    case 0xc2:
    case 0xca:
    case 0xd2:
    case 0xda:
      *address = GetArg2(instruction);
      break;
    // JP (HL)
    case 0xe9:
      *address = hl_value;
      break;
    // JR N
    case 0x18:
      *address = current_address + static_cast<int8_t>(GetArg1(instruction));
      break;
    // JR CC, N
    case 0x20:
    case 0x28:
    case 0x30:
    case 0x38:
      *address = current_address + static_cast<int8_t>(GetArg2(instruction));
      break;
    // CALL NN
    case 0xcd:
      *address = GetArg1(instruction);
      break;
    // CALL CC, NN
    case 0xc4:
    case 0xcc:
    case 0xd4:
    case 0xdc:
      *address = GetArg2(instruction);
      break;
    // RST
    case 0xc7:
      *address = 0x0000;
      break;
    case 0xcf:
      *address = 0x0008;
      break;
    case 0xd7:
      *address = 0x0010;
      break;
    case 0xdf:
      *address = 0x0018;
      break;
    case 0xe7:
      break;
      *address = 0x0020;
    case 0xef:
      *address = 0x0028;
      break;
    case 0xf7:
      *address = 0x0030;
      break;
    case 0xff:
      *address = 0x0038;
      break;
    // RET
    case 0xc9:
      *address = return_value;
      break;
    // RET CC
    case 0xc0:
    case 0xc8:
    case 0xd0:
    case 0xd8:
      *address = return_value;
      break;
    // RETI
    case 0xd9:
      *address = return_value;
      break;
    default:
      LOG(FATAL) << "Not a valid jump command: 0x" << std::hex << instruction.instruction;
  }
  return true;
}
} // namespace

OpcodeParser::OpcodeParser(const memory::MemoryMapper& memory_mapper) :
    rom_bridge_(memory_mapper), dma_bridge_(memory_mapper) {}

void OpcodeParser::Init() {
  DecompilerFactory factory;
  factory.set_rom(&rom_bridge_);
  factory.set_type(DecompilerFactory::UNFORMATTED_ROM);
  rom_decompiler_ = factory.Build();
  rom_decompiler_->Decompile();
}

void OpcodeParser::Reset() {
  DecompilerFactory factory;
  factory.set_rom(&rom_bridge_);
  factory.set_type(DecompilerFactory::FORMATTED_ROM);
  rom_decompiler_ = factory.Build();
  rom_decompiler_->Decompile();
}

void OpcodeParser::InitDMADecompiler(uint16_t high_ram_address) {
  DecompilerFactory factory;
  factory.set_rom(&dma_bridge_);
  factory.set_type(DecompilerFactory::NONE);
  dma_decompiler_ = factory.Build();
  dma_decompiler_->AddPathStart(high_ram_address);
  dma_decompiler_->Decompile();

  std::stringstream dma_code;
  dma_decompiler_->PrintToStream(&dma_code);
  LOG(INFO) << "Completed compilation of DMA code:\n" << dma_code.str();
}

bool OpcodeParser::FetchInstruction(uint16_t address, 
                                    uint16_t return_value, 
                                    uint16_t hl_value, 
                                    Instruction* instruction) {
  if (!is_dma_running_) {
    return FetchInstructionROM(address, return_value, hl_value, instruction);
  } else {
    return FetchInstructionDMA(address, return_value, hl_value, instruction);
  }
}

bool OpcodeParser::FetchInstructionROM(uint16_t address, 
                                       uint16_t return_value, 
                                       uint16_t hl_value, 
                                       Instruction* instruction) {
  if (rom_decompiler_->LookUp(address, instruction)) {
    uint16_t jump_address;
    if (GetJumpAddress(*instruction, address, return_value, hl_value, &jump_address)) {
      if (jump_address >= memory::kHighRAMMin) {
        LOG(INFO) << "Jumping to high RAM for DMA Transfer.";
        is_dma_running_ = true;
        InitDMADecompiler(jump_address);
      } else if (instruction->instruction == 0xe9) {
        LOG(INFO) << "Exploring new code path.";
        rom_decompiler_->AddPathStart(jump_address);
        rom_decompiler_->Decompile();

        ofstream file("out.s");
        rom_decompiler_->PrintToStream(&file);
      }
    }
    return true;
  } else {
    LOG(WARNING) << "Could not find valid instruction at given address.";
    return false;
  }
}

bool OpcodeParser::FetchInstructionDMA(uint16_t address, 
                                       uint16_t return_value, 
                                       uint16_t hl_value, 
                                       Instruction* instruction) {
  if (dma_decompiler_->LookUp(address, instruction)) {
    uint16_t jump_address;
    if (GetJumpAddress(*instruction, address, return_value, hl_value, &jump_address)) {
      if (jump_address <= memory::kROMBankNMax) {
        LOG(INFO) << "Jumping back to ROM from DMA Transfer.";
        is_dma_running_ = false;
        dma_decompiler_.reset();
      } else if (instruction->instruction == 0xe9) {
        LOG(INFO) << "Exploring new code path.";
        dma_decompiler_->AddPathStart(jump_address);
        dma_decompiler_->Decompile();
      }
    }
    return true;
  } else {
    LOG(WARNING) << "Could not find valid instruction at given address.";
    return false;
  }
}

} // namespace opcode_executor
} // namespace backend
