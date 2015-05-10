#include "backend/opcode_executor/opcode_executor.h"

#include "backend/decompiler/instruction.h"
#include "backend/decompiler/rom_reader.h"
#include "submodules/glog/src/glog/logging.h"

namespace back_end {
namespace opcode_executor {

using opcodes::Opcode;
using decompiler::Instruction;
using decompiler::ROMReader;

OpcodeExecutor::OpcodeExecutor(std::unique_ptr<memory::MemoryMapper> memory_mapper, 
                               memory::PrimaryFlags* primary_flags) :
    memory_mapper_(std::move(memory_mapper)),
    bridge_(*memory_mapper_),
    rom_reader_(new ROMReader(bridge_)),
    interrupt_enable_(primary_flags->interrupt_enable()),
    interrupt_flag_(primary_flags->interrupt_flag()) {}

OpcodeExecutor::~OpcodeExecutor() = default;

int OpcodeExecutor::ReadInstruction() {
  HandleInterrupts();

  Instruction instruction;
  rom_reader_->Read(cpu_.rPC, &instruction);
  cpu_.rPC += instruction.instruction_width_bytes;
  OpcodeHandler handler = opcode_map_[instruction.instruction];

  ExecutorContext context;
  context.instruction_ptr = &cpu_.rPC;
  context.interrupt_master_enable = &interrupt_master_enable_;
  context.memory_mapper = memory_mapper_.get();
  context.cpu = &cpu_;

  int handler_result = handler(instruction, &context);
  if (handler_result == -1) {
    return -1;
  } else {
    cpu_.rPC = handler_result;
    return 0;
  }
}

// int OpcodeExecutor::OldReadInstruction() {
//   HandleInterrupts(); // Before a fetch we must check for and handle interrupts.
//   unsigned short opcode_address = cpu_.rPC;
//   unsigned short instruction_ptr = cpu_.rPC;
//   unsigned short opcode = memory_mapper_->Read(instruction_ptr);
//   instruction_ptr++;
//   unsigned short next_byte = memory_mapper_->Read(instruction_ptr);
// 
//   // This is jank.
//   unsigned char magic = 0;
//   if (opcode == 0xCB && (next_byte & 0b11000000) > 0) {
//     instruction_ptr++;
//     magic = (0b00111000 & next_byte) >> 3;
//     opcode = (opcode << 8) | (next_byte & 0b11000111);
//   } else if (opcode == 0xCB) {
//     instruction_ptr++;
//     opcode = (opcode << 8) | next_byte;
//   } else if (next_byte == 0x10 && opcode == 0x00) {
//     instruction_ptr++;
//     opcode = next_byte << 8 | opcode;
//   }
//   cpu_.rPC = instruction_ptr;
// 
//   Opcode opcode_struct;
//   auto opcode_iter = opcode_map_.find(opcode);
//   if (opcode_iter == opcode_map_.end()) {
//     LOG(ERROR) << "Opcode instruction, " << std::hex << opcode << ", does not exist. Next value is " << std::hex << next_byte;
//     return -1; // Let the clocktroller know that we cannot continue.
//   } else {
//     LOG(INFO) << "Fetched opcode: " << std::hex << opcode << " address: " << std::hex << opcode_address;
//     LOG(INFO) << "A is " << std::hex << std::hex << 0x0000 + cpu_.flag_struct.rA;
//     LOG(INFO) << "C is " << std::hex << std::hex << 0x0000 + cpu_.bc_struct.rC;
//     LOG(INFO) << "HL is " << std::hex << std::hex << 0x0000 + cpu_.rHL;
//     opcode_struct = opcode_iter->second;
//   }
// 
//   ExecutorContext context(&interrupt_master_enable_,
//                           &cpu_.rPC,
//                           &opcode_struct,
//                           memory_mapper_.get(),
//                           &cpu_,
//                           magic,
//                           opcode_address);
//   
//   // XXX(Brendan): A hack to poke tetris.
//   // if (opcode_address == 0x034c && opcode_struct.opcode_name == 0xf0) {
//   //   LOG(INFO) << "TETRIS HACK!!!!";
//   //   memory_mapper_.Write(0xff80, 0x00);
//   // }
//   // if (opcode_address == 0x36c && opcode_struct.opcode_name == 0xf0) {
//   //   LOG(INFO) << "TETRIS HACK!!!!";
//   //   memory_mapper_.Write(0xff85, 0xff);
//   // }
//   
//   int handler_result = opcode_struct.handler(&context);
//   if (handler_result == -1) {
//     return -1;
//   } else {
//     cpu_.rPC = handler_result;
//   }
// 
//   return context.opcode->clock_cycles;
// }

// 1) Check interrupt_master_enable_ (IME)
// 2) Check to see if any interrupt flags that are enabled
// 3) Push PC (as if CALL was performed), set PC to interrupt address, disable IME
void OpcodeExecutor::HandleInterrupts() {
  if (interrupt_master_enable_ && CheckInterrupts()) {
    interrupt_master_enable_ = false;

    if (interrupt_flag_->v_blank() && interrupt_enable_->v_blank()) {
      LOG(INFO) << "Handling V blank interrupt.";
      interrupt_flag_->set_v_blank(false);
      cpu_.rPC = 0x0040;
    } else if (interrupt_flag_->lcd_stat() && interrupt_enable_->lcd_stat()) {
      LOG(INFO) << "Handling LCD stat interrupt.";
      interrupt_flag_->set_lcd_stat(false);
      cpu_.rPC = 0x0048;
    } else if (interrupt_flag_->timer() && interrupt_enable_->timer()) {
      LOG(INFO) << "Handling timer interrupt.";
      interrupt_flag_->set_timer(false);
      cpu_.rPC = 0x0050;
    } else if (interrupt_flag_->serial() && interrupt_enable_->serial()) {
      LOG(INFO) << "Handling serial interrupt.";
      interrupt_flag_->set_serial(false);
      cpu_.rPC = 0x0058;
    } else if (interrupt_flag_->joypad() && interrupt_enable_->joypad()) {
      LOG(INFO) << "Handling joypad interrupt.";
      interrupt_flag_->set_joypad(false);
      cpu_.rPC = 0x0060;
    }
  }
}

bool OpcodeExecutor::CheckInterrupts() {
  return (interrupt_flag_->v_blank() && interrupt_enable_->v_blank()) ||
      (interrupt_flag_->lcd_stat() && interrupt_enable_->lcd_stat()) ||
      (interrupt_flag_->timer() && interrupt_enable_->timer()) ||
      (interrupt_flag_->serial() && interrupt_enable_->serial()) ||
      (interrupt_flag_->joypad() && interrupt_enable_->joypad());
}

} // namespace handlers
} // namespace back_end
