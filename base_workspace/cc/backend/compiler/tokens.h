#ifndef TURBO_SANTA_COMMON_BACKEND_TOKENS_H_
#define TURBO_SANTA_COMMON_BACKEND_TOKENS_H_

#include "cc/backend/decompiler/instruction.h"

namespace backend {
namespace compiler {

enum class TokenType : int {
  OPCODE = 256,
  REGISTER,
  NUMBER,
};

union TokenValue {
  TokenValue() {}
  TokenValue(decompiler::Opcode opcode_) : opcode(opcode_) {}
  TokenValue(decompiler::Register reg_) : reg(reg_) {}
  TokenValue(uint16_t number_) : number(number_) {}

  decompiler::Opcode opcode;
  decompiler::Register reg;
  uint16_t number;
};

} // namespace compiler
} // backend

#endif // TURBO_SANTA_COMMON_BACKEND_TOKENS_H_
