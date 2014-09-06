#include "back_end/opcode_handlers.h"

namespace back_end {
namespace handlers {

unsigned char* registers = { cpu.rB, cpu.rC, cpu.rD, cpu.rE, cpu.rH, cpu.rL, cpu.rHL, cpu.rA }
unsigned char* shortRegs = { cpu.rBC, cpu.rDE, cpu.rHL, cpu.rSP, cpu.rAF }

int Add8Bit(unsigned char* rom, int instruction_ptr, Opcode opcode) {
	int index = rom[instruction_ptr] - opcode.opcode_name;
	cpu.rA += registers[index];
	return instruction_ptr + 1;
}

int Add8BitLiteral(unsigned char* rom, instruction_ptr, Opcode opcode) {
	cpu.rA += rom[instruction_ptr + 1];
	return instruction_ptr + 2;
}

int Dec8Bit(unsigned char* rom, int instruction_ptr, int opcode_length) {
    ALU8BitRegisterRegister* opcode = static_cast<ALU8BitRegisterRegister*>(rom + instruction_ptr);
}

} // namespace handlers
} // namespace back_end
