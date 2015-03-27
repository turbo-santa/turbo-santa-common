#ifndef TURBO_SANTA_COMMON_BACK_END_OPCODE_PARSER_H_
#define TURBO_SANTA_COMMON_BACK_END_OPCODE_PARSER_H_

#include "backend/config.h"

#include <map>
#include <memory>
#include "backend/memory/interrupt_flag.h"
#include "backend/memory/memory_mapper.h"
#include "backend/memory/primary_flags.h"
#include "backend/opcode_executor/opcodes.h"
#include "backend/opcode_executor/opcode_map.h"
#include "backend/opcode_executor/registers.h"

namespace back_end {
namespace handlers {

class OpcodeExecutor {
 public:
  OpcodeExecutor(std::unique_ptr<memory::MemoryMapper> memory_mapper, 
                 memory::PrimaryFlags* primary_flags) :
      memory_mapper_(std::move(memory_mapper)),
      interrupt_enable_(primary_flags->interrupt_enable()),
      interrupt_flag_(primary_flags->interrupt_flag()) {}

  int ReadInstruction();

 private:
  bool CheckInterrupts();
  void HandleInterrupts();
    
  registers::GB_CPU cpu_;
  std::unique_ptr<memory::MemoryMapper> memory_mapper_;
  std::map<unsigned short, opcodes::Opcode> opcode_map_ = opcodes::CreateOpcodeMap(&cpu_);
  // This is a special flag/register that can only be set or unset and can
  // only be accessed by the user using the EI, DI or RETI instructions.
  bool interrupt_master_enable_ = false;
  memory::InterruptEnable* interrupt_enable_;
  memory::InterruptFlag* interrupt_flag_;
};

struct ExecutorContext {
  ExecutorContext(bool* interrupt_master_enable_,
                  unsigned short* instruction_ptr_, 
                  opcodes::Opcode* opcode_, 
                  memory::MemoryMapper* memory_mapper_, 
                  registers::GB_CPU* cpu_,
                  unsigned char magic_,
                  unsigned short instruction_address_) :
      interrupt_master_enable(interrupt_master_enable_),
      instruction_ptr(instruction_ptr_),
      opcode(opcode_),
      memory_mapper(memory_mapper_), 
      cpu(cpu_),
      magic(magic_),
      instruction_address(instruction_address_) {}

  ExecutorContext(ExecutorContext* context) : 
      interrupt_master_enable(context->interrupt_master_enable),
      instruction_ptr(context->instruction_ptr),
      opcode(context->opcode),
      memory_mapper(context->memory_mapper),
      cpu(context->cpu),
      magic(context->magic),
      instruction_address(context->instruction_address) {}

  bool* interrupt_master_enable;
  unsigned short* instruction_ptr;
  opcodes::Opcode* opcode;
  memory::MemoryMapper* memory_mapper;
  registers::GB_CPU* cpu;
  unsigned char magic;
  unsigned short instruction_address;
};

} // namespace handlers
} // namespace back_end
#endif // TURBO_SANTA_COMMON_BACK_END_OPCODE_PARSER_H_
