#ifndef TURBO_SANTA_COMMON_BACK_END_OPCODE_HANDLERS_H_
#define TURBO_SANTA_COMMON_BACK_END_OPCODE_HANDLERS_H_
#include "registers.h"

namespace back_end {
namespace handlers {

int Add8Bit(unsigned char* rom, int instruction_ptr, Opcode opcode);

int Add8BitLiteral(unsigned char* rom, int instruction_ptr, Opcode opcode);

int Dec8Bit(unsigned char* rom, int instruction_ptr, Opcode opcode);

} // namespace back_end
} // namespace handlers

#endif // TURBO_SANTA_COMMON_BACK_END_OPCODE_HANDLERS_H_
