#ifndef TURBO_SANTA_COMMON_BACKEND_ROM_READER_H_
#define TURBO_SANTA_COMMON_BACKEND_ROM_READER_H_

#include <map>
#include <memory>
#include <vector>
#include "backend/decompiler/instruction.h"
#include "backend/decompiler/instruction_factory.h"
#include "backend/decompiler/instruction_map.h"
#include "backend/decompiler/raw_instruction.h"

namespace back_end {
namespace decompiler {

class ROMReader {
 public:
  ROMReader(std::unique_ptr<std::vector<uint8_t>> rom) : rom_(std::move(rom)) {}

  Instruction Read(uint16_t address);

 private:
  std::map<uint16_t, InstructionFactory> instruction_map_ = CreateInstructionMap();
  std::unique_ptr<std::vector<uint8_t>> rom_;
  RawInstruction8Bit raw_instruction_8bit_;
  RawInstruction16Bit raw_instruction_16bit_;
  RawInstruction24Bit raw_instruction_24bit_;

  const RawInstructionBase& raw_instruction(uint16_t address, ValueWidth width);
};

} // namespace decompiler
} // namespace decompiler

#endif // TURBO_SANTA_COMMON_BACKEND_ROM_READER_H_
