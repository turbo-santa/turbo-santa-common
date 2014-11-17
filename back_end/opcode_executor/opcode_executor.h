#ifndef TURBO_SANTA_COMMON_BACK_END_OPCODE_PARSER_H_
#define TURBO_SANTA_COMMON_BACK_END_OPCODE_PARSER_H_

#include <map>
#include <memory>

#include "back_end/memory/memory_mapper.h"
#include "back_end/opcode_executor/opcode_handlers.h"
#include "back_end/opcode_executor/opcodes.h"
#include "back_end/opcode_executor/opcode_map.h"
#include "back_end/opcode_executor/registers.h"

namespace test_harness {
class TestHarness;
} // namespace test_harness

namespace back_end {
namespace handlers {

class OpcodeExecutor {
  public: 
    OpcodeExecutor(unsigned char*, long);
    void ReadInstruction();

  private:
    bool CheckInterrupts();
    void HandleInterrupts();
    
    memory::MemoryMapper memory_mapper_;
    registers::GB_CPU cpu_;
    std::map<unsigned short, Opcode> opcode_map;
    // This is a special flag/register that can only be set or unset and can
    // only be accessed by the user using the EI, DI or RETI instructions.
    bool interrupt_master_enable_ = false;

    friend class test_harness::TestHarness;
};

struct ExecutorContext {
  ExecutorContext(bool* interrupt_master_enable_,
                  unsigned short* instruction_ptr_, 
                  Opcode* opcode_, 
                  MemoryMapper* memory_mapper_, 
                  registers::GB_CPU* cpu_) : 
      interrupt_master_enable(interrupt_master_enable_),
      instruction_ptr(instruction_ptr_),
      opcode(opcode_),
      memory_mapper(memory_mapper_), 
      cpu(cpu_) {}

  ExecutorContext(ExecutorContext* context) : 
      interrupt_master_enable(context->interrupt_master_enable),
      instruction_ptr(context->instruction_ptr),
      opcode(context->opcode),
      memory_mapper(context->memory_mapper),
      cpu(context->cpu) {}

  bool* interrupt_master_enable;
  unsigned short* instruction_ptr;
  Opcode* opcode;
  MemoryMapper* memory_mapper;
  registers::GB_CPU* cpu;
};


} // namespace handlers
} // namespace back_end
#endif // TURBO_SANTA_COMMON_BACK_END_OPCODE_PARSER_H_
