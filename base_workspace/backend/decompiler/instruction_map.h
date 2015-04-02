#ifndef TURBO_SANTA_COMMON_BACKEND_INSTRUCTION_MAP_H_
#define TURBO_SANTA_COMMON_BACKEND_INSTRUCTION_MAP_H_

#include "backend/config.h"

#include <cstdint>
#include <map>
#include <string>
#include "backend/decompiler/instruction.h"
#include "backend/decompiler/instruction_factory.h"
#include "backend/decompiler/raw_instruction.h"

namespace back_end {
namespace decompiler {

std::map<uint16_t, InstructionFactory> CreateInstructionMap();

std::map<Opcode, std::string> CreateNameMap();

} // namespace decompiler
} // namespace decompiler

#endif // TURBO_SANTA_COMMON_BACKEND_INSTRUCTION_MAP_H_
