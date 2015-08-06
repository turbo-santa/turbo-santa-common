#ifndef TURBO_SANTA_COMMON_BACK_END_OPCODE_EXECUTOR_OPCODE_PARSER_H_
#define TURBO_SANTA_COMMON_BACK_END_OPCODE_EXECUTOR_OPCODE_PARSER_H_

#include <cstdint>
#include <memory>
#include "cc/backend/memory/memory_mapper_rom_bridge.h"

namespace back_end {
namespace decompiler {
class Decompiler;
class Instruction;
class ROMReader;
} // namespace decompiler
} // namespace back_end

namespace back_end {
namespace memory {
class MemoryMapper;
} // namespace memory
} // namespace back_end

namespace back_end {
namespace opcode_executor {

class OpcodeParser {
 public:
  OpcodeParser(const memory::MemoryMapper& memory_mapper);
  void Init();
  void Reset();
  bool FetchInstruction(uint16_t address,
                        uint16_t return_value,
                        uint16_t hl_value,
                        decompiler::Instruction* instruction);

  bool is_dma_running() { return is_dma_running_; }

 private:
  void InitDMADecompiler(uint16_t high_ram_address);

  bool FetchInstructionROM(uint16_t address,
                           uint16_t return_value,
                           uint16_t hl_value,
                           decompiler::Instruction* instruction);

  bool FetchInstructionDMA(uint16_t address,
                           uint16_t return_value,
                           uint16_t hl_value,
                           decompiler::Instruction* instruction);

  memory::MemoryMapperROMBridge rom_bridge_;
  std::unique_ptr<decompiler::Decompiler> rom_decompiler_;
  memory::MemoryMapperHighRAMBridge dma_bridge_;
  std::unique_ptr<decompiler::Decompiler> dma_decompiler_;
  bool is_dma_running_ = false;
};

} // namespace opcode_executor
} // namespace back_end

#endif // TURBO_SANTA_COMMON_BACK_END_OPCODE_EXECUTOR_OPCODE_PARSER_H_
