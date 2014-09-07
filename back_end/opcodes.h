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
    unsigned char* reg1;
    unsigned char* reg2;
    OpcodeHandler handler;
};

struct PartialOpcode {
    unsigned char opcode_name;
    unsigned char* reg1;
};

struct PartialOpcodeExtraReg {
    unsigned char opcode_name;
    unsigned char* reg1;
    unsigned char* reg2;
};

std::vector<Opcode> ToList2(std::vector<PartialOpcodeExtraReg> partial_opcodes, OpcodeHandler handler) {
    std::vector<Opcode> full_opcodes;
    for (PartialOpcodeExtraReg partial_opcode : partial_opcodes) {
        full_opcodes.push_back({partial_opcode.opcode_name, partial_opcode.reg1, partial_opcode.reg2, handler});
    }
    return full_opcodes;
}

std::vector<Opcode> ToList1(std::vector<PartialOpcode> partial_opcodes, OpcodeHandler handler) {
    std::vector<Opcode> full_opcodes;
    for (PartialOpcode partial_opcode : partial_opcodes) {
        full_opcodes.push_back({partial_opcode.opcode_name, partial_opcode.reg1, nullptr, handler});
    }
    return full_opcodes;
}

std::vector<Opcode> ToList(Opcode opcode) {
    return {opcode};
}

// // TODO(Brendan, Diego): Use this function for creating handlers that share opcodes.
// // The handler can differentiate on input from inpsecting the name of the opcode
// // parameter.
// std::vector<Opcode> CreateOpcodeList(std::vector<unsigned char> opcode_names, OpcodeHandler handler, int length) {
//     std::vector<Opcode> return_list;
//     for (unsigned char opcode_name : opcode_names) {
//         return_list.push_back({opcode_name, length, handler});
//     }
//     return return_list;
// }

} // namespace back_end
} // namespace opcodes

#endif // TURBO_SANTA_COMMON_BACK_END_OPCODES_H_
