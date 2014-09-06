#ifndef TURBO_SANTA_COMMON_BACK_END_OPCODES_H_
#define TURBO_SANTA_COMMON_BACK_END_OPCODES_H_

#include <functional>

namespace back_end {
namespace opcodes {

template<class T> using OpcodeHandler = std::function<void(T)>;
template<class T> using OpcodeParser = std::function<T(unsigned char*)>;

template<class T>
struct Opcode {
    char opcode_name;
    OpcodeParser<T> parser;
    OpcodeHandler<T> handler;
};

namespace arithmetic_opcodes {
}
}
}

#endif // TURBO_SANTA_COMMON_BACK_END_OPCODES_H_
