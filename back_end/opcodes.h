#ifndef TURBO_SANTA_COMMON_BACK_END_OPCODES_H_
#define TURBO_SANTA_COMMON_BACK_END_OPCODES_H_

#include <functional>
#include <vector>

namespace back_end {
namespace opcodes {

struct Arithmetic8Bit;
int Add8BitHandler(int, int);

typedef std::function<int(unsigned char* rom, int instruction_ptr, int opcode_length)> OpcodeHandler;

struct Opcode {
    unsigned char opcode_name;
    int length;
    OpcodeHandler handler;
};

} // namespace back_end
} // namespace opcodes

#endif // TURBO_SANTA_COMMON_BACK_END_OPCODES_H_
