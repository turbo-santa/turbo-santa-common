#ifndef TURBO_SANTA_COMMON_BACK_END_OPCODES_H_
#define TURBO_SANTA_COMMON_BACK_END_OPCODES_H_

#include "back_end/config.h"

#include <functional>
#include <map>
#include <string>
#include <vector>

namespace back_end {
namespace handlers {
struct ExecutorContext;
}
}

namespace back_end {
namespace opcodes {

typedef std::function<int(handlers::ExecutorContext* context)> OpcodeHandler;

struct Opcode {
    unsigned short opcode_name;
    std::string opcode_name_str;
    unsigned short* reg1;
    std::string reg1_str;
    unsigned short* reg2;
    std::string reg2_str;
    OpcodeHandler handler;
    unsigned int clock_cycles;
};

struct PartialOpcode {
    unsigned short opcode_name;
    std::string opcode_name_str;
    unsigned short* reg1;
    std::string reg1_str;
    unsigned int clock_cycles;
};

struct PartialOpcodeExtraReg {
    unsigned short opcode_name;
    std::string opcode_name_str;
    unsigned short* reg1;
    std::string reg1_str;
    unsigned short* reg2;
    std::string reg2_str;
    unsigned int clock_cycles;
};

struct OpcodeResult {
    int instruction_ptr;
    unsigned int clock_cycles;
};

std::vector<Opcode> ToList2(std::vector<PartialOpcodeExtraReg> partial_opcodes, OpcodeHandler handler); 

std::vector<Opcode> ToList1(std::vector<PartialOpcode> partial_opcodes, OpcodeHandler handler);

std::vector<Opcode> ToList(Opcode opcode);

std::vector<Opcode> Flatten(std::vector<std::vector<Opcode>> to_flatten);

std::map<unsigned short, Opcode> ToMap(std::vector<Opcode> opcode_list);

} // namespace back_end
} // namespace opcodes

#endif // TURBO_SANTA_COMMON_BACK_END_OPCODES_H_
