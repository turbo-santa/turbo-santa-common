#include "cc/backend/opcode_executor/opcode_executor.h"

#include "cc/backend/decompiler/decompiler.h"
#include "cc/backend/decompiler/decompiler_factory.h"
#include "cc/backend/decompiler/instruction.h"
#include "cc/backend/decompiler/rom_reader.h"
#include "cc/backend/opcode_executor/opcode_handlers.h"
#include "glog/logging.h"

namespace backend {
namespace opcode_executor {

using opcodes::Opcode;
using decompiler::ArgumentType;
using decompiler::DecompilerFactory;
using decompiler::Instruction;
using decompiler::Parameter;
using decompiler::Register;
using decompiler::ROMReader;
using registers::GB_CPU;

OpcodeExecutor::OpcodeExecutor(memory::MemoryMapper* memory_mapper, 
                               memory::PrimaryFlags* primary_flags,
                               memory::Flag* internal_rom_flag) : 
    memory_mapper_(memory_mapper),
    opcode_parser_(*memory_mapper_),
    interrupt_enable_(primary_flags->interrupt_enable()),
    interrupt_flag_(primary_flags->interrupt_flag()), 
    internal_rom_flag_(internal_rom_flag) {
  cpu_.rPC = 0;
}

OpcodeExecutor::~OpcodeExecutor() = default;

int OpcodeExecutor::ReadInstruction() {
  HandleInterrupts();
  if (halted_) {
    // We need to use some number of clock cycles while halted.
    return 4;
  }

  LOG(INFO) << "Current address: 0x" << std::hex << cpu_.rPC;

  if (using_internal_rom_ && internal_rom_flag_->flag()) {
    SwitchToExternalROM();
    LOG(INFO) << "Switched to external ROM.";
    using_internal_rom_ = false;
  }

  Instruction instruction;
  if (!opcode_parser_.FetchInstruction(cpu_.rPC,
                                       PollRegister(memory_mapper_, cpu_.rSP),
                                       cpu_.rHL,
                                       &instruction)) {
    LOG(WARNING) << "Invalid address, 0x" << std::hex << cpu_.rPC 
        << ", exiting with error.";
    return -1;
  }
  cpu_.rPC += instruction.instruction_width_bytes;
  OpcodeHandler handler = opcode_map_[instruction.instruction];

  ExecutorContext context;
  context.instruction_ptr = &cpu_.rPC;
  context.interrupt_master_enable = &interrupt_master_enable_;
  context.halted = &halted_;
  context.memory_mapper = memory_mapper_;
  context.cpu = &cpu_;

  int handler_result = handler(instruction, &context);
  if (handler_result == -1) {
    return -1;
  } else {
    cpu_.rPC = static_cast<uint16_t>(handler_result);
    return instruction.clock_cycles;
  }
}

// 1) Check interrupt_master_enable_ (IME)
// 2) Check to see if any interrupt flags that are enabled
// 3) Push PC (as if CALL was performed), set PC to interrupt address, disable IME
void OpcodeExecutor::HandleInterrupts() {
  if (interrupt_master_enable_ && CheckInterrupts()) {
    // TODO(Brendan): Find a better solution to interrupts durring a DMA transfer.
    if (opcode_parser_.is_dma_running()) {
      return;
    }
    interrupt_master_enable_ = false;
    halted_ = false;
    PushRegister(memory_mapper_, &cpu_, &cpu_.rPC);

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
} // namespace backend
