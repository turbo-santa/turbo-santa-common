#ifndef TURBO_SANTA_COMMON_BACKEND_DECOMPILER_H_
#define TURBO_SANTA_COMMON_BACKEND_DECOMPILER_H_

#include "backend/config.h"

#include <cstdint>
#include <map>
#include <memory>
#include <ostream>
#include <set>
#include <stack>
#include <vector>

#include "backend/decompiler/instruction.h"
#include "backend/decompiler/instruction_map.h"
#include "backend/decompiler/rom_reader.h"

namespace back_end {
namespace decompiler {

class Decompiler {
 public:
  Decompiler(std::unique_ptr<std::vector<uint8_t>> rom) :
      rom_reader_(std::move(rom)) {}

  void Decompile();

  void PrintToStream(std::ostream* out_stream);

 private:
  ROMReader rom_reader_;
  std::map<uint16_t, Instruction> address_opcode_map_;
  std::map<uint16_t, const Instruction*> allocation_map_;
  std::stack<uint16_t> code_paths_;
  std::set<uint16_t> addresses_jumped_to_;

  std::map<Opcode, std::string> opcode_map_ = CreateNameMap();
  std::map<Register, std::string> register_map_ = CreateRegisterMap();

  void DecompileInstructionAt(uint16_t address);
  void AddInstruction(uint16_t address, Instruction instruction);
  std::string PrintInstruction(uint16_t address);
};

} // namespace decompiler
} // namespace decompiler

#endif // TURBO_SANTA_COMMON_BACKEND_DECOMPILER_H_
