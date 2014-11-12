#include "back_end/opcode_executor/opcode_executor.h"

namespace back_end {
namespace handlers {

using std::unique_ptr;
using opcodes::opcode_map;
using opcodes::Opcode;
using memory::MemoryMapper;

OpcodeExecutor::OpcodeExecutor(unsigned char*, long) {
    handlers::mem_map = new MemoryMapper();
    rom_ = handlers::mem_map->get_pointer();
}

void OpcodeExecutor::ReadInstruction() {
    unsigned short opcode = rom_[instruction_ptr_];
    if (opcode == 0xCB || opcode == 0x10) {
        instruction_ptr_++;
        unsigned char opcode_lb = rom_[instruction_ptr_];
        opcode = opcode << 8 | opcode_lb;
    }
    Opcode opcode_struct = opcode_map[opcode];
    instruction_ptr_ = opcode_struct.handler(rom_, instruction_ptr_, opcode_struct);
}

} // namespace handlers
} // namespace back_end
