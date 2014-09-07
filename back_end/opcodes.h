#ifndef TURBO_SANTA_COMMON_BACK_END_OPCODES_H_
#define TURBO_SANTA_COMMON_BACK_END_OPCODES_H_

#include <functional>
#include <map>
#include <vector>

namespace back_end {
namespace opcodes {

struct Opcode;

typedef std::function<int(unsigned char* rom, int instruction_ptr, Opcode opcode)> OpcodeHandler;

struct Opcode {
    unsigned short opcode_name;
    unsigned char* reg1;
    unsigned char* reg2;
    OpcodeHandler handler;
};

struct PartialOpcode {
    unsigned short opcode_name;
    unsigned char* reg1;
};

struct PartialOpcodeExtraReg {
    unsigned short opcode_name;
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

std::vector<Opcode> Flatten(std::vector<std::vector<Opcode>> to_flatten) {
    std::vector<Opcode> flattened;
    for (std::vector<Opcode> opcodes : to_flatten) {
        for (Opcode opcode : opcodes) {
            flattened.push_back(opcode);
        }
    }
    return flattened;
}

std::map<unsigned short, Opcode> ToMap(std::vector<Opcode> opcode_list) {
    std::map<unsigned short, Opcode> opcode_map;
    for (Opcode opcode : opcode_list) {
        opcode_map[opcode.opcode_name] = opcode;
    }
    return opcode_map;
}

} // namespace back_end
} // namespace opcodes

#endif // TURBO_SANTA_COMMON_BACK_END_OPCODES_H_
