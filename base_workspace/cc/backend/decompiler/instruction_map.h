#ifndef TURBO_SANTA_COMMON_BACKEND_INSTRUCTION_MAP_H_
#define TURBO_SANTA_COMMON_BACKEND_INSTRUCTION_MAP_H_

#include <cstdint>
#include <map>
#include <string>
#include "cc/backend/decompiler/instruction.h"
#include "cc/backend/decompiler/instruction_factory.h"
#include "cc/backend/decompiler/raw_instruction.h"

namespace backend {
namespace decompiler {

std::map<uint16_t, InstructionFactory> CreateInstructionMap();

std::map<Opcode, std::string> CreateNameMap();

std::map<Register, std::string> CreateRegisterMap();

} // namespace decompiler
} // namespace backend

#endif // TURBO_SANTA_COMMON_BACKEND_INSTRUCTION_MAP_H_
