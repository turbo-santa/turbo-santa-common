#ifndef TURBO_SANTA_COMMON_BACK_END_OPCODES_H_
#define TURBO_SANTA_COMMON_BACK_END_OPCODES_H_

#include <functional>
#include <vector>

namespace back_end {
namespace opcodes {

template <class T> struct Opcode;
struct Arithmetic8Bit;
int Add8BitHandler(int, int);

typedef std::function<int(unsigned char* rom, int instruction_ptr, int opcode_length)> OpcodeHandler;
template<class T> using OpcodeList = std::vector<Opcode<T>>;

template<class T>
struct Opcode {
    unsigned char opcode_name;
    int length;
    OpcodeHandler handler;
};

} // namespace back_end
} // namespace opcodes

#endif // TURBO_SANTA_COMMON_BACK_END_OPCODES_H_
