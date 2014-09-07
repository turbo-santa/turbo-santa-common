#include "back_end/opcodes.h"
#include "back_end/opcode_map.h"
#include "registers.h"

namespace back_end {
namespace opcode_parser {

using opcodes::opcode_map;
using opcodes::Opcode;

class OpcodeParser {
    int instruction_ptr_;
    unsigned char* rom_;
    
  public: 
    OpcodeParser(unsigned char* rom);
    void ReadInstruction();
};

OpcodeParser::OpcodeParser(unsigned char* rom) {
    rom_ = rom;
}

void OpcodeParser::ReadInstruction() {
    unsigned short opcode = rom_[instruction_ptr_];
    Opcode opcode_struct = opcode_map[opcode];
    instruction_ptr_ = opcode_struct.handler(rom_, instruction_ptr_, opcode_struct);
}

} // namespace opcode_parser
} // namespace back_end
