#ifndef TURBO_SANTA_COMMON_BACK_END_OPCODES_H_
#define TURBO_SANTA_COMMON_BACK_END_OPCODES_H_

#include <functional>
#include <vector>

namespace back_end {
namespace opcodes {

struct Opcode;

typedef std::function<int(unsigned char* rom, int instruction_ptr, Opcode opcode)> OpcodeHandler;

struct Opcode {
    unsigned char opcode_name;
    int length;
    OpcodeHandler handler;
};

// TODO(Brendan, Diego): Use this function for creating handlers that share opcodes.
// The handler can differentiate on input from inpsecting the name of the opcode
// parameter.
std::vector<Opcode> CreateOpcodeList(std::vector<unsigned char> opcode_names, OpcodeHandler handler, int length) {
    std::vector<Opcode> return_list;
    for (unsigned char opcode_name : opcode_names) {
        return_list.push_back({opcode_name, length, handler});
    }
    return return_list;
}

} // namespace back_end
} // namespace opcodes

#endif // TURBO_SANTA_COMMON_BACK_END_OPCODES_H_
