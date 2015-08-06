#ifndef TURBO_SANTA_COMMON_BACKEND_ROM_READER_H_
#define TURBO_SANTA_COMMON_BACKEND_ROM_READER_H_

#include <map>
#include <memory>
#include <vector>
#include "cc/backend/decompiler/instruction.h"
#include "cc/backend/decompiler/instruction_factory.h"
#include "cc/backend/decompiler/instruction_map.h"
#include "cc/backend/decompiler/raw_instruction.h"
#include "cc/backend/decompiler/rom_bridge.h"

namespace back_end {
namespace decompiler {

class ROMReader {
 public:
  ROMReader(const ROMBridge& rom) : rom_(rom) {}

  bool Read(uint16_t address, Instruction* instruction);

 private:
  std::map<uint16_t, InstructionFactory> instruction_map_ = CreateInstructionMap();
  const ROMBridge& rom_;
  RawInstruction8Bit raw_instruction_8bit_;
  RawInstruction16Bit raw_instruction_16bit_;
  RawInstruction24Bit raw_instruction_24bit_;

  const RawInstructionBase& raw_instruction(uint16_t address, ValueWidth width);
};

} // namespace decompiler
} // namespace decompiler

#endif // TURBO_SANTA_COMMON_BACKEND_ROM_READER_H_
