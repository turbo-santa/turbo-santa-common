#ifndef TURBO_SANTA_COMMON_BACK_END_OPCODE_EXECUTOR_OPCODE_EXECUTOR_H_
#define TURBO_SANTA_COMMON_BACK_END_OPCODE_EXECUTOR_OPCODE_EXECUTOR_H_

#include <map>
#include <memory>
#include "cc/backend/memory/interrupt_flag.h"
#include "cc/backend/memory/memory_mapper.h"
#include "cc/backend/memory/memory_mapper_rom_bridge.h"
#include "cc/backend/memory/primary_flags.h"
#include "cc/backend/opcode_executor/executor_context.h"
#include "cc/backend/opcode_executor/opcodes.h"
#include "cc/backend/opcode_executor/opcode_map.h"
#include "cc/backend/opcode_executor/opcode_parser.h"
#include "cc/backend/opcode_executor/registers.h"

namespace back_end {
namespace decompiler {
class Decompiler;
class ROMReader;
} // namespace decompiler
} // namespace back_end

namespace back_end {
namespace opcode_executor {

class OpcodeExecutor {
 public:
  OpcodeExecutor(std::unique_ptr<memory::MemoryMapper> memory_mapper, 
                 memory::PrimaryFlags* primary_flags,
                 memory::Flag* internal_rom_flag);

  ~OpcodeExecutor();

  void Init() { opcode_parser_.Init(); }

  int ReadInstruction();

 private:
  bool CheckInterrupts();
  void HandleInterrupts();
  void SwitchToExternalROM() { opcode_parser_.Reset(); }
    
  registers::GB_CPU cpu_;
  std::unique_ptr<memory::MemoryMapper> memory_mapper_;
  OpcodeParser opcode_parser_;
  std::map<uint16_t, OpcodeHandler> opcode_map_ = CreateOpcodeMap();
  // This is a special flag/register that can only be set or unset and can
  // only be accessed by the user using the EI, DI or RETI instructions.
  bool interrupt_master_enable_ = false;
  bool halted_ = false;
  bool using_internal_rom_ = true;
  memory::InterruptEnable* interrupt_enable_;
  memory::InterruptFlag* interrupt_flag_;
  memory::Flag* internal_rom_flag_;

  friend test_harness::TestHarness;
};

} // namespace opcode_executor
} // namespace back_end
#endif // TURBO_SANTA_COMMON_BACK_END_OPCODE_EXECUTOR_OPCODE_EXECUTOR_H_
