#include "back_end/opcodes.h"
#include "registers.h"

class OpcodeParser {
    int instruction_ptr_;
    char* rom_;
    
  public: 
    OpcodeParser(char* rom);
    void ReadInstruction();
};

OpcodeParser::OpcodeParser(char* rom) {
    rom_ = rom;
}

void OpcodeParser::ReadInstruction() {
    char opcode = rom_[instruction_ptr_];
    back_end::opcodes::Opcode opcode_struct = opcode_map[opcode];
    instruction_ptr_ = opcode_struct.handler(rom_, cpu_, instruction_ptr_, opcode_struct.length);
}
