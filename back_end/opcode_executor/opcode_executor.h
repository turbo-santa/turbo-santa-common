#ifndef TURBO_SANTA_COMMON_BACK_END_OPCODE_PARSER_H_
#define TURBO_SANTA_COMMON_BACK_END_OPCODE_PARSER_H_

#include "back_end/config.h"

#include <map>
#include <memory>

#include "back_end/debugger/call_stack.h"
#include "back_end/debugger/deltas.h"
#include "back_end/debugger/frames.h"
#include "back_end/graphics/graphics_controller.h"
#include "back_end/graphics/screen.h"
#include "back_end/memory/memory_mapper.h"
#include "back_end/opcode_executor/opcode_handlers.h"
#include "back_end/opcode_executor/opcodes.h"
#include "back_end/opcode_executor/opcode_map.h"
#include "back_end/opcode_executor/registers.h"

namespace test_harness {
class TestHarness;
} // namespace test_harness

namespace back_end {
namespace clocktroller {
class ClocktrollerTest;
} // namespace clocktroller
} // namespace back_end

namespace back_end {
namespace handlers {

class OpcodeExecutor {
  public: 
    OpcodeExecutor();
    OpcodeExecutor(graphics::Screen* screen,
                   debugger::GreatLibrary* great_library,
                   unsigned char* rom,
                   long rom_size);
    int ReadInstruction();

  private:
    bool CheckInterrupts();
    void HandleInterrupts();
    
    memory::MemoryMapper memory_mapper_;
    graphics::GraphicsController graphics_controller_;
    registers::GB_CPU cpu_;
    std::map<unsigned short, opcodes::Opcode> opcode_map;
    // This is a special flag/register that can only be set or unset and can
    // only be accessed by the user using the EI, DI or RETI instructions.
    bool interrupt_master_enable_ = false;
    debugger::RegisterProducer register_producer_;
    debugger::PCProducer pc_producer_;
    debugger::FrameFactory frame_factory_;
    debugger::CallStack call_stack_;

    bool load_ly_to_a_ = false;
    bool last_instruction_was_ldhan_ = false;


    friend class test_harness::TestHarness;
    friend class back_end::clocktroller::ClocktrollerTest;
};

struct ExecutorContext {
  ExecutorContext(bool* interrupt_master_enable_,
                  unsigned short* instruction_ptr_, 
                  opcodes::Opcode* opcode_, 
                  memory::MemoryMapper* memory_mapper_, 
                  registers::GB_CPU* cpu_,
                  unsigned char magic_,
                  debugger::FrameFactory* frame_factory_,
                  unsigned short instruction_address_,
                  debugger::CallStack* call_stack_,
                  bool* load_ly_to_a_,
                  bool* last_instruction_was_ldhan_) : 
      interrupt_master_enable(interrupt_master_enable_),
      instruction_ptr(instruction_ptr_),
      opcode(opcode_),
      memory_mapper(memory_mapper_), 
      cpu(cpu_),
      magic(magic_),
      frame_factory(frame_factory_),
      instruction_address(instruction_address_),
      call_stack(call_stack_),
      load_ly_to_a(load_ly_to_a_),
      last_instruction_was_ldhan(last_instruction_was_ldhan_) {}

  ExecutorContext(ExecutorContext* context) : 
      interrupt_master_enable(context->interrupt_master_enable),
      instruction_ptr(context->instruction_ptr),
      opcode(context->opcode),
      memory_mapper(context->memory_mapper),
      cpu(context->cpu),
      magic(context->magic),
      frame_factory(context->frame_factory),
      instruction_address(context->instruction_address),
      call_stack(context->call_stack),
      load_ly_to_a(context->load_ly_to_a),
      last_instruction_was_ldhan(context->last_instruction_was_ldhan){}

  bool* interrupt_master_enable;
  unsigned short* instruction_ptr;
  opcodes::Opcode* opcode;
  memory::MemoryMapper* memory_mapper;
  registers::GB_CPU* cpu;
  unsigned char magic;
  debugger::FrameFactory* frame_factory;
  unsigned short instruction_address;
  debugger::CallStack* call_stack;
  bool* load_ly_to_a;
  bool* last_instruction_was_ldhan;
};


} // namespace handlers
} // namespace back_end
#endif // TURBO_SANTA_COMMON_BACK_END_OPCODE_PARSER_H_
