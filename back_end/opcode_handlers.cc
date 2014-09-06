#include "back_end/opcode_handlers.h"

namespace back_end {
namespace handlers {

unsigned char* registers = { cpu.rB, cpu.rC, cpu.rD, cpu.rE, cpu.rH, cpu.rL, cpu.rHL, cpu.rA }
unsigned char* shortRegs = { cpu.rBC, cpu.rDE, cpu.rHL, cpu.rSP, cpu.rAF }

unsigned char NthBit(unsigned int byte, int n) {
	return (byte >> n) & 1;
}

bool DoesOverflow(unsigned int left, unsigned int right, int bit) {
	if (NthBit(left, bit) && NthBit(right, bit)) {
		return true;
	}
	if (NthBit(left, bit) || NthBit(right, bit)) {
		if (!NthBit(left + right, bit)) {
			return true;
		}
	}
	return false;
}

bool DoesUnderflow(unsigned int left, unsigned int right, int bit) {
	if (!NthBit(left, bit) && (NthBit(right, bit) || NthBit(left - right, bit)) {
		return true;
	}
	return false;
}

bool DoesBorrow8(unsigned char left, unsigned char right) {
	return !DoesUnderflow(left, right, 7);
}

bool DoesBorrow16(unsigned char left, unsigned char right) {
	return DoesUnderflow(left, right, 15);
}

bool DoesHalfBorrow(unsigned char left, unsigned char right) {
	return DoesUnderflow(left, right, 3);
}

bool DoesHalfCarry(unsigned char left, unsigned char right) {
	return DoesOverflow(left, right, 3);
}

bool DoesCarry8(unsigned char left, unsigned char right) {
	return DoesOverflow(left, right, 7);
}

bool DoesCarry16(unsigned int left, unsigned int right) {
	return DoesOverflow(left, right, 15);
}

int Add8Bit(unsigned char* rom, int instruction_ptr, Opcode opcode) {
	int index = rom[instruction_ptr] - opcode.opcode_name;
	Add8Bit(registers[index]);
	return instruction_ptr + 1;
}

int Add8BitLiteral(unsigned char* rom, instruction_ptr, Opcode opcode) {
	Add8Bit(rom[instruction_ptr + 1]);
	return instruction_ptr + 2;
}

void Add8Bit(unsigned char value) {
	cpu.rF.C = DoesCarry8(cpu.rA, value);
	cpu.rF.H = DoesHalfCarry(cpu.rA, value);
	cpu.rA += value;
	cpu.rF.Z = (cpu.rA == 0);
	cpu.rF.N = 0;
}

int ADC8Bit(unsigned char* rom, int instruction_ptr, Opcode opcode) {
	int index = rom[instruction_ptr] - opcode.opcode_name;
	ADC8Bit(registers[index]);
	return instruction_ptr + 1;
}

int ADC8BitLiteral(unsigned char* rom, int instruction_ptr, Opcode opcode) {
	char literal = rom[instruction_ptr + 1];
	ADC8Bit(literal);
	return instruction_ptr + 2;
}

void ADC8Bit(unsigned char value) {
	char carry = cpu.rF.C;
	cpu.rF.C = DoesCarry8(cpu.rA, value);
	cpu.rF.H = DoesHalfCarry(cpu.rA, value);
	cpu.rA += value;
	cpu.rF.C |= DoesCarry8(cpu.rA, value);
	cpu.rF.H |= DoesHalfCarry(cpu.rA, value);
	cpu.rF.Z = (cpu.rA == 0);
	cpu.rF.N = 0;
}

int Sub8Bit(unsigned char* rom, int instruction_ptr, Opcode opcode) {
	int index = rom[instruction_ptr] - opcode.opcode_name;
	Sub8Bit(registers[index]);
	return instruction_ptr + 1;
}

int Sub8BitLiteral(unsigned char* rom, int instruction_ptr, Opcode opcode) {
	char literal = rom[instruction_ptr + 1];
	Sub8Bit(literal);
	return instruction_ptr + 2;
}

void Sub8Bit(unsigned char value) {
	cpu.rF.C = !DoesBorrow8(cpu.rA, value);
	cpu.rF.H = !DoesHalfBorrow(cpu.rA, value);
	cpu.rF.N = 1;
	cpu.rA -= value;
	cpu.rF.Z = (cpu.rA == 0);
}

int Dec8Bit(unsigned char* rom, int instruction_ptr, int opcode_length) {
    ALU8BitRegisterRegister* opcode = static_cast<ALU8BitRegisterRegister*>(rom + instruction_ptr);
}

} // namespace handlers
} // namespace back_end
