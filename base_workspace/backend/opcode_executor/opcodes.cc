#include "back_end/config.h"

#include "back_end/opcode_executor/opcodes.h"

namespace back_end {
namespace opcodes {

std::vector<Opcode> ToList2(std::vector<PartialOpcodeExtraReg> partial_opcodes, OpcodeHandler handler) {
    std::vector<Opcode> full_opcodes;
    for (PartialOpcodeExtraReg partial_opcode : partial_opcodes) {
        full_opcodes.push_back(
            {partial_opcode.opcode_name,
            partial_opcode.reg1,
            partial_opcode.reg2,
            handler,
            partial_opcode.clock_cycles});
    }
    return full_opcodes;
}

std::vector<Opcode> ToList1(std::vector<PartialOpcode> partial_opcodes, OpcodeHandler handler) {
    std::vector<Opcode> full_opcodes;
    for (PartialOpcode partial_opcode : partial_opcodes) {
        full_opcodes.push_back(
            {partial_opcode.opcode_name,
            partial_opcode.reg1,
            nullptr,
            handler,
            partial_opcode.clock_cycles});
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
