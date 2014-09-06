#ifndef TURBO_SANTA_COMMON_BACK_END_OPCODE_HANDLERS_H_
#define TURBO_SANTA_COMMON_BACK_END_OPCODE_HANDLERS_H_
#include "registers.h"

namespace back_end {
namespace handlers {

struct ALU8BitRegisterRegister {
    unsigned char opcode;
    unsigned char target_register;
    unsigned char source_register;
};

int Add8Bit(unsigned char* rom, GB_CPU cpu, int instruction_ptr, int opcode_length);

int Dec8Bit(unsigned char* rom, int instruction_ptr, int opcode_length);

} // namespace back_end
} // namespace handlers

#endif // TURBO_SANTA_COMMON_BACK_END_OPCODE_HANDLERS_H_
