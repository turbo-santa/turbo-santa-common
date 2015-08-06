#ifndef TURBO_SANTA_COMMON_BACK_END_OPCODE_EXECUTOR_EXECUTOR_CONTEXT_H_
#define TURBO_SANTA_COMMON_BACK_END_OPCODE_EXECUTOR_EXECUTOR_CONTEXT_H_

#include <cstdint>
#include "cc/backend/opcode_executor/registers.h"
#include "cc/backend/memory/memory_mapper.h"

namespace back_end {
namespace opcode_executor {

struct ExecutorContext {
  ExecutorContext() = default;

  ExecutorContext(bool* interrupt_master_enable_,
                  bool* halted_,
                  uint16_t* instruction_ptr_, 
                  memory::MemoryMapper* memory_mapper_, 
                  registers::GB_CPU* cpu_) :
      interrupt_master_enable(interrupt_master_enable_),
      halted(halted_),
      instruction_ptr(instruction_ptr_),
      memory_mapper(memory_mapper_), 
      cpu(cpu_) {}

  ExecutorContext(ExecutorContext* context) : 
      interrupt_master_enable(context->interrupt_master_enable),
      halted(context->halted),
      instruction_ptr(context->instruction_ptr),
      memory_mapper(context->memory_mapper),
      cpu(context->cpu) {}

  bool* interrupt_master_enable;
  bool* halted;
  uint16_t* instruction_ptr;
  memory::MemoryMapper* memory_mapper;
  registers::GB_CPU* cpu;
};

} // namespace opcode_executor
} // namespace back_end
#endif // TURBO_SANTA_COMMON_BACK_END_OPCODE_EXECUTOR_EXECUTOR_CONTEXT_H_
