#include "back_end/config.h"

#include "back_end/debugger/great_library.h"
#include "back_end/opcode_executor/opcode_executor.h"
#include "back_end/memory/interrupt_flag.h"
#include "back_end/opcode_executor/opcode_handlers.h"
#include "back_end/opcode_executor/registers.h"
#include <glog/logging.h>

namespace back_end {
namespace handlers {

using std::unique_ptr;
using debugger::GreatLibrary;
using graphics::Screen;
using graphics::ScreenRaster;
using opcodes::CreateOpcodeMap;
using opcodes::Opcode;
using memory::MemoryMapper;
using memory::InterruptEnable;
using memory::InterruptFlag;

class NullScreen : public Screen {
 public:
  virtual void Draw(const ScreenRaster&) {}
}; 

OpcodeExecutor::OpcodeExecutor() :
    graphics_controller_(&memory_mapper_, new NullScreen()),
    register_producer_(&cpu_),
    pc_producer_(&cpu_.rPC),
    frame_factory_(new GreatLibrary(),
                   &register_producer_,
                   memory_mapper_.memory_producer(),
                   &pc_producer_) {
  cpu_.rPC = 0x0000;
  opcode_map = CreateOpcodeMap(&cpu_);
}

OpcodeExecutor::OpcodeExecutor(Screen* screen, GreatLibrary* great_library, unsigned char* rom, long rom_size) : 
    memory_mapper_(rom, rom_size, true), 
    graphics_controller_(&memory_mapper_, screen),
    register_producer_(&cpu_),
    pc_producer_(&cpu_.rPC),
    frame_factory_(great_library,
                   &register_producer_,
                   memory_mapper_.memory_producer(),
                   &pc_producer_) {
  cpu_.rPC = 0x0000;
  opcode_map = CreateOpcodeMap(&cpu_);
}

int OpcodeExecutor::ReadInstruction() {
  HandleInterrupts(); // Before a fetch we must check for and handle interrupts.
  unsigned short opcode_address = cpu_.rPC;
  unsigned short instruction_ptr = cpu_.rPC;
  unsigned short opcode = memory_mapper_.Read(instruction_ptr);
  instruction_ptr++;
  unsigned short next_byte = memory_mapper_.Read(instruction_ptr);

  // This is jank.
  unsigned char magic = 0;
  if (opcode == 0xCB && (next_byte & 0b11000000) > 0) {
    instruction_ptr++;
    magic = (0b00111000 & next_byte) >> 3;
    opcode = (opcode << 8) | (next_byte & 0b11000111);
  } else if (opcode == 0xCB) {
    instruction_ptr++;
    opcode = (opcode << 8) | next_byte;
  } else if (next_byte == 0x10 && opcode == 0x00) {
    instruction_ptr++;
    opcode = next_byte << 8 | opcode;
  }
  cpu_.rPC = instruction_ptr;

  Opcode opcode_struct;
  auto opcode_iter = opcode_map.find(opcode);
  if (opcode_iter == opcode_map.end()) {
    LOG(ERROR) << "Opcode instruction, " << std::hex << opcode << ", does not exist. Next value is " << std::hex << next_byte;
    return -1; // Let the clocktroller know that we cannot continue.
  } else {
    LOG(INFO) << "Fetched opcode: " << std::hex << opcode << " address: " << std::hex << opcode_address;
    LOG(INFO) << "A is " << std::hex << std::hex << 0x0000 + cpu_.flag_struct.rA;
    LOG(INFO) << "C is " << std::hex << std::hex << 0x0000 + cpu_.bc_struct.rC;
    LOG(INFO) << "HL is " << std::hex << std::hex << 0x0000 + cpu_.rHL;
    opcode_struct = opcode_iter->second;
  }
  if (load_ly_to_a_) {
    last_instruction_was_ldhan_ = true;
  }
  load_ly_to_a_ = false;

  ExecutorContext context(&interrupt_master_enable_,
                          &cpu_.rPC,
                          &opcode_struct,
                          &memory_mapper_,
                          &cpu_,
                          magic,
                          &frame_factory_,
                          opcode_address,
                          &call_stack_,
                          &load_ly_to_a_,
                          &last_instruction_was_ldhan_);
  // XXX(Brendan): A hack to poke tetris.
  // if (opcode_address == 0x034c && opcode_struct.opcode_name == 0xf0) {
  //   LOG(INFO) << "TETRIS HACK!!!!";
  //   memory_mapper_.Write(0xff80, 0x00);
  // }
  // if (opcode_address == 0x36c && opcode_struct.opcode_name == 0xf0) {
  //   LOG(INFO) << "TETRIS HACK!!!!";
  //   memory_mapper_.Write(0xff85, 0xff);
  // }
  int handler_result = opcode_struct.handler(&context);
  if (handler_result == -1) {
    frame_factory_.SubmitFrame();
    return -1;
  } else {
    cpu_.rPC = handler_result;
  }

  graphics_controller_.Tick(opcode_struct.clock_cycles);

  if (opcode_address == 0x100 && memory_mapper_.interrupt_flag()) {
    pc_producer_.Clear();
  }

  frame_factory_.SetRawInstruction(opcode);
  frame_factory_.SubmitFrame();

  return context.opcode->clock_cycles;
}

// 1) Check interrupt_master_enable_ (IME)
// 2) Check to see if any interrupt flags that are enabled
// 3) Push PC (as if CALL was performed), set PC to interrupt address, disable IME
void OpcodeExecutor::HandleInterrupts() {
  if (interrupt_master_enable_ && CheckInterrupts()) {
    call_stack_.Push({frame_factory_.current_timestamp(), cpu_.rPC});
    handlers::PushRegister(&memory_mapper_, &cpu_, &cpu_.rPC);
    interrupt_master_enable_ = false;

    InterruptFlag* interrupt_flag = memory_mapper_.interrupt_flag();
    InterruptEnable* interrupt_enable = memory_mapper_.interrupt_enable();
    if (interrupt_flag->v_blank() && interrupt_enable->v_blank()) {
      LOG(INFO) << "Handling V blank interrupt.";
      interrupt_flag->set_v_blank(false);
      cpu_.rPC = 0x0040;
    } else if (interrupt_flag->lcd_stat() && interrupt_enable->lcd_stat()) {
      LOG(INFO) << "Handling LCD stat interrupt.";
      interrupt_flag->set_lcd_stat(false);
      cpu_.rPC = 0x0048;
    } else if (interrupt_flag->timer() && interrupt_enable->timer()) {
      LOG(INFO) << "Handling timer interrupt.";
      interrupt_flag->set_timer(false);
      cpu_.rPC = 0x0050;
    } else if (interrupt_flag->serial() && interrupt_enable->serial()) {
      LOG(INFO) << "Handling serial interrupt.";
      interrupt_flag->set_serial(false);
      cpu_.rPC = 0x0058;
    } else if (interrupt_flag->joypad() && interrupt_enable->joypad()) {
      LOG(INFO) << "Handling joypad interrupt.";
      interrupt_flag->set_joypad(false);
      cpu_.rPC = 0x0060;
    }
  }
}

bool OpcodeExecutor::CheckInterrupts() {
  InterruptFlag* interrupt_flag = memory_mapper_.interrupt_flag();
  InterruptEnable* interrupt_enable = memory_mapper_.interrupt_enable();
  return (interrupt_flag->v_blank() && interrupt_enable->v_blank()) ||
      (interrupt_flag->lcd_stat() && interrupt_enable->lcd_stat()) ||
      (interrupt_flag->timer() && interrupt_enable->timer()) ||
      (interrupt_flag->serial() && interrupt_enable->serial()) ||
      (interrupt_flag->joypad() && interrupt_enable->joypad());
}

} // namespace handlers
} // namespace back_end
