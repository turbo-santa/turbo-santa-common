#ifndef TURBO_SANTA_COMMON_BACK_END_OPCODE_EXECUTOR_OPCODE_EXECUTOR_H_
#define TURBO_SANTA_COMMON_BACK_END_OPCODE_EXECUTOR_OPCODE_EXECUTOR_H_

#include <map>
#include <memory>
#include "backend/memory/interrupt_flag.h"
#include "backend/memory/memory_mapper.h"
#include "backend/memory/memory_mapper_rom_bridge.h"
#include "backend/memory/primary_flags.h"
#include "backend/opcode_executor/executor_context.h"
#include "backend/opcode_executor/opcodes.h"
#include "backend/opcode_executor/opcode_map.h"
#include "backend/opcode_executor/registers.h"

namespace back_end {
namespace decompiler {
class ROMReader;
} // namespace decompiler
} // namespace back_end

namespace back_end {
namespace opcode_executor {

class OpcodeExecutor {
 public:
  OpcodeExecutor(std::unique_ptr<memory::MemoryMapper> memory_mapper, 
                 memory::PrimaryFlags* primary_flags);

  ~OpcodeExecutor();

  int ReadInstruction();

  int OldReadInstruction();

 private:
  bool CheckInterrupts();
  void HandleInterrupts();
    
  registers::GB_CPU cpu_;
  std::unique_ptr<memory::MemoryMapper> memory_mapper_;
  memory::MemoryMapperROMBridge bridge_;
  std::unique_ptr<decompiler::ROMReader> rom_reader_;
  std::map<uint16_t, OpcodeHandler> opcode_map_ = CreateOpcodeMap();
  // This is a special flag/register that can only be set or unset and can
  // only be accessed by the user using the EI, DI or RETI instructions.
  bool interrupt_master_enable_ = false;
  memory::InterruptEnable* interrupt_enable_;
  memory::InterruptFlag* interrupt_flag_;
};

} // namespace opcode_executor
} // namespace back_end
#endif // TURBO_SANTA_COMMON_BACK_END_OPCODE_EXECUTOR_OPCODE_EXECUTOR_H_
