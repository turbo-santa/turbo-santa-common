#include "back_end/opcode_executor/opcode_executor.h"

#include "back_end/opcode_executor/registers.h"

namespace back_end {
namespace handlers {

using std::unique_ptr;
using opcodes::opcode_map;
using opcodes::Opcode;
using memory::MemoryMapper;

void OpcodeExecutor::ReadInstruction() {
    unsigned short opcode = memory_mapper_.Read(instruction_ptr_);
    if (opcode == 0xCB || opcode == 0x10) {
        instruction_ptr_++;
        unsigned char opcode_lb = memory_mapper_.Read(instruction_ptr_);
        opcode = opcode << 8 | opcode_lb;
    }
    Opcode opcode_struct = opcode_map[opcode];

    ExecutorContext context(&instruction_ptr_, &opcode_struct, &memory_mapper_, &cpu_);
    instruction_ptr_ = opcode_struct.handler(&context);
}

} // namespace handlers
} // namespace back_end
