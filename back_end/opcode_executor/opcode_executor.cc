#include "back_end/opcode_executor/opcode_executor.h"

#include "back_end/opcode_executor/registers.h"

namespace back_end {
namespace registers {
GB_CPU* cpu;
}
}

namespace back_end {
namespace handlers {

using std::unique_ptr;
using opcodes::opcode_map;
using opcodes::Opcode;
using memory::MemoryMapper;

OpcodeExecutor::OpcodeExecutor(unsigned char*, long) {
  registers::cpu = &cpu_;
  // TODO(Diego): It acutally starts at something like 0x100.
  cpu_.rPC = 0x0000;
}

void OpcodeExecutor::ReadInstruction() {
    unsigned short instruction_ptr = cpu_.rPC;
    unsigned short opcode = memory_mapper_.Read(instruction_ptr);
    if (opcode == 0xCB || opcode == 0x10) {
        instruction_ptr++;
        unsigned char opcode_lb = memory_mapper_.Read(instruction_ptr);
        opcode = opcode << 8 | opcode_lb;
    }
    cpu_.rPC = instruction_ptr;
    Opcode opcode_struct = opcode_map[opcode];

    ExecutorContext context(&cpu_.rPC, &opcode_struct, &memory_mapper_, &cpu_);
    cpu_.rPC = opcode_struct.handler(&context);
}

} // namespace handlers
} // namespace back_end
