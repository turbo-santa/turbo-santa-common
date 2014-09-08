#ifndef TURBO_SANTA_COMMON_BACK_END_OPCODE_PARSER_H_
#define TURBO_SANTA_COMMON_BACK_END_OPCODE_PARSER_H_

#include <memory>

#include "back_end/memory/memory_mapper.h"
#include "back_end/opcode_handlers.h"
#include "back_end/opcodes.h"
#include "back_end/opcode_map.h"
#include "back_end/registers.h"

namespace back_end {
namespace opcode_parser {

using std::unique_ptr;
using memory::MemoryMapper;
using opcodes::opcode_map;
using opcodes::Opcode;

class OpcodeParser {
    // TODO(Diego): It acutally starts at something like 0x100.
    int instruction_ptr_ = 0;
    unsigned char* rom_;
    
  public: 
    OpcodeParser(unsigned char* rom, long rom_size);
    void ReadInstruction();
};

OpcodeParser::OpcodeParser(unsigned char* rom, long rom_size) {
    handlers::mem_map = unique_ptr<MemoryMapper>(new MemoryMapper(rom, rom_size));
    rom_ = handlers::mem_map->get_pointer();
}

void OpcodeParser::ReadInstruction() {
    unsigned short opcode = rom_[instruction_ptr_];
    if (opcode == 0xCB || opcode == 0x10) {
        instruction_ptr_++;
        unsigned char opcode_lb = rom_[instruction_ptr_];
        opcode = opcode << 8 | opcode_lb;
    }
    Opcode opcode_struct = opcode_map[opcode];
    instruction_ptr_ = opcode_struct.handler(rom_, instruction_ptr_, opcode_struct);
}

} // namespace opcode_parser
} // namespace back_end
#endif // TURBO_SANTA_COMMON_BACK_END_OPCODE_PARSER_H_
