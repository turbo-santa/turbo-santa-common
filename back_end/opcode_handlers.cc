#include "back_end/opcode_handlers.h"
#include "back_end/opcodes.h"

namespace back_end {
namespace handlers {

using back_end::opcodes::Opcode;

unsigned char* registers = { cpu.rB, cpu.rC, cpu.rD, cpu.rE, cpu.rH, cpu.rL, cpu.rHL, cpu.rA };
unsigned char* shortRegs = { cpu.rBC, cpu.rDE, cpu.rHL, cpu.rSP, cpu.rAF };

unsigned char* GetRegister(unsigned char* rom, int instruction_ptr, unsigned char opcode_name) {
    unsigned char opcode = rom[instruction_ptr];
    unsigned char register_index = opcode - opcode_name;
    return registers + register_index;
}

unsigned short* GetRegister16(unsigned char* rom, int instruction_ptr, unsigned char opcode_name) {
	unsigned char opcode = rom[instruction_ptr];
	unsigned char register_index = (opcode >> 4);
	return shortRegs + register_index;
}

unsigned char GetRegisterValue(unsigned char* rom, int instruction_ptr, unsigned char opcode_name) {
    unsigned char opcode = rom[instruction_ptr];
    unsigned char register_index = opcode - opcode_name;
    return registers[register_index];
}

unsigned short GetRegisterValue16(unsigned char* rom, int instruction_ptr, unsigned char opcode_name) {
	unsigned char opcode = rom[instruction_ptr];
	unsigned char register_index = (opcode >> 4);
	return shortRegs[register_index];
}

unsigned char GetParameterValue(unsigned char* rom, int instruction_ptr) {
    return rom[instruction_ptr + 1];
}

unsigned short GetParameterValue16(unsigned char* rom, int instruction_ptr) {
	return (((short)rom[instruction_ptr + 1]) << 8) | (short)rom[instruction_ptr + 2];
}

void SetZFlag(unsigned char register_value) {
    cpu.rF.Z = register_value == 0;
}

void SetNFlag(bool performed_subtraction) {
    cpu.rF.N = performed_subtraction;
}

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

bool DoesBorrow16(unsigned short left, unsigned short right) {
	return DoesUnderflow(left, right, 15);
}

bool DoesHalfBorrow8(unsigned char left, unsigned char right) {
	return DoesUnderflow(left, right, 3);
}

bool DoesHalfBorrow16(unsigned short left, unsigned short right) {
	return DoesUnderflow(left, right, 11);
}

bool DoesHalfCarry8(unsigned char left, unsigned char right) {
	return DoesOverflow(left, right, 3);
}

bool DoesCarry8(unsigned char left, unsigned char right) {
	return DoesOverflow(left, right, 7);
}

bool DoesHalfCarry16(unsigned char left, unsigned char right) {
	return DoesOverflow(left, right, 11);
}

bool DoesCarry16(unsigned int left, unsigned int right) {
	return DoesOverflow(left, right, 15);
}

int Add8Bit(unsigned char* rom, int instruction_ptr, Opcode opcode) {
	Add8Bit(GetRegisterValue(rom, instruction_ptr, opcode.opcode_name));
	return instruction_ptr + 1;
}

int Add8BitLiteral(unsigned char* rom, instruction_ptr, Opcode opcode) {
	Add8Bit(GetParameterValue(rom, instruction_ptr));
	return instruction_ptr + 2;
}

void Add8Bit(unsigned char value) {
	cpu.rF.C = DoesCarry8(cpu.rA, value);
	cpu.rF.H = DoesHalfCarry8(cpu.rA, value);
	cpu.rA += value;
	SetZFlag(cpu.rA);
	SetNFlag(false);
}

int ADC8Bit(unsigned char* rom, int instruction_ptr, Opcode opcode) {
	ADC8Bit(GetRegisterValue(rom, instruction_ptr, opcode.opcode_name));
	return instruction_ptr + 1;
}

int ADC8BitLiteral(unsigned char* rom, int instruction_ptr, Opcode opcode) {
	ADC8Bit(GetParameterValue(rom, instruction_ptr));
	return instruction_ptr + 2;
}

void ADC8Bit(unsigned char value) {
	char carry = cpu.rF.C;
	cpu.rF.C = DoesCarry8(cpu.rA, value);
	cpu.rF.H = DoesHalfCarry8(cpu.rA, value);
	cpu.rA += value;
	cpu.rF.C |= DoesCarry8(cpu.rA, value);
	cpu.rF.H |= DoesHalfCarry8(cpu.rA, value);
	SetZFlag(cpu.rA);
	SetNFlag(false);
}

int Sub8Bit(unsigned char* rom, int instruction_ptr, Opcode opcode) {
	Sub8Bit(GetRegisterValue(rom, instruction_ptr, opcode.opcode_name));
	return instruction_ptr + 1;
}

int Sub8BitLiteral(unsigned char* rom, int instruction_ptr, Opcode opcode) {
	Sub8Bit(GetParameterValue(rom, instruction_ptr));
	return instruction_ptr + 2;
}

void Sub8Bit(unsigned char value) {
	cpu.rF.C = !DoesBorrow8(cpu.rA, value);
	cpu.rF.H = !DoesHalfBorrow8(cpu.rA, value);
	cpu.rA -= value;
	SetZFlag(cpu.rA);
	SetNFlag(true);
}

int And8Bit(unsigned char* rom, int instruction_ptr, Opcode opcode) {
    cpu.rA &= GetRegisterValue(rom, instruction_ptr, opcode.opcode_name);
    SetZFlag(cpu.rA);
    SetNFlag(false);
    cpu.rF.H = 0;
    cpu.rF.C = 0;
    return instruction_ptr + 1;
}

int And8BitLiteral(unsigned char* rom, int instruction_ptr, Opcode opcode) {
    cpu.rA &= GetParameterValue(rom, instruction_ptr);
    SetZFlag(cpu.rA);
    SetNFlag(false);
    cpu.rF.H = 0;
    cpu.rF.C = 0;
    return instruction_ptr + 1;
}

int Or8Bit(unsigned char* rom, int instruction_ptr, Opcode opcode) {
    cpu.rA |= GetRegisterValue(rom, instruction_ptr, opcode.opcode_name);
    SetZFlag(cpu.rA);
    SetNFlag(false);
    cpu.rF.H = 0;
    cpu.rF.C = 0;
    return instruction_ptr + 1;
}

int Or8BitLiteral(unsigned char* rom, int instruction_ptr, Opcode opcode) {
    cpu.rA |= GetParameterValue(rom, instruction_ptr);
    SetZFlag(cpu.rA);
    SetNFlag(false);
    cpu.rF.H = 0;
    cpu.rF.C = 0;
    return instruction_ptr + 1;
}

int Xor8Bit(unsigned char* rom, int instruction_ptr, Opcode opcode) {
    cpu.rA ^= GetRegisterValue(rom, instruction_ptr, opcode.opcode_name);
    SetZFlag(cpu.rA);
    SetNFlag(false);
    cpu.rF.H = 0;
    cpu.rF.C = 0;
    return instruction_ptr + 1;
}

int Xor8BitLiteral(unsigned char* rom, int instruction_ptr, Opcode opcode) {
    cpu.rA ^= GetParameterValue(rom, instruction_ptr);
    SetZFlag(cpu.rA);
    SetNFlag(false);
    cpu.rF.H = 0;
    cpu.rF.C = 0;
    return instruction_ptr + 1;
}

int Cp8Bit(unsigned char* rom, int instruction_ptr, Opcode opcode) {
    unsigned char result = cpu.rA - GetRegisterValue(rom, instruction_ptr, opcode);
    SetZFlag(result);
    SetNFlag(true); // Performed subtraction.
    cpu.rF.H = NthBit(cpu.rA, 4) != NthBit(result, 4);
    cpu.rF.C = NthBit(cpu.rA, 7) != NthBit(result, 7);
    return instruction_ptr + 1;
}

int Cp8BitLiteral(unsigned char* rom, int instruction_ptr, Opcode opcode) {
    unsigned char result = cpu.rA - GetParameterValue(rom, instruction_ptr);
    SetZFlag(result);
    SetNFlag(true); // Performed subtraction.
    cpu.rF.H = NthBit(cpu.rA, 4) != NthBit(result, 4);
    cpu.rF.C = NthBit(cpu.rA, 7) != NthBit(result, 7);
    return instruction_ptr + 1;
}

int Inc8Bit(unsigned char* rom, int instruction_ptr, Opcode opcode) {
    unsigned char* reg = GetRegister(rom, instruction_ptr, opcode.opcode_name);
    unsigned char forth_bit = NthBit(*reg, 3);
    ++(*reg);
    bool borrowed_h = forth_bit != NthBit(*reg, 3);
    SetZFlag(*reg)
    SetNFlag(false);
    cpu.rF.H = borrowed_h;
    return instruction_ptr + 1;
}

int Dec8Bit(unsigned char* rom, int instruction_ptr, Opcode opcode) {
    unsigned char* reg = GetRegister(rom, instruction_ptr, opcode.opcode_name);
    unsigned char fourth_bit = NthBit(*reg, 4);
    --(*reg);
    bool borrowed_h = forth_bit != NthBit(*reg, 4);
    SetZFlag(*reg);
    SetNFlag(true);
    cpu.rF.H = borrowed_h;
    return instruction_ptr + 1;
}

int Add16Bit(unsigned char* rom, int instruction_ptr, Opcode opcode) {
	short value = GetRegisterValue16(rom, instruction_ptr, opcode.opcode_name);
	cpu.rF.H = DoesHalfCarry16(cpu.rA, value);
	cpu.rF.C = DoesCarry16(cpu.rA, value);
	cpu.rHL += value;
	SetNFlag(false);
	return instruction_ptr + 1;
}

int AddSPLiteral(unsigned char* rom, int instruction_ptr, Opcode opcode) {
	unsigned char value = GetParameterValue(rom, instruction_ptr);
	if (NthBit(value, 7)) {
		cpu.rF.H = DoesHalfBorrow16(cpu.rSP, value);
		cpu.rF.C = DoesBorrow16(cpu.rSP, value);
	} else {
		cpu.rF.H = DoesHalfCarry16(cpu.rSP, value);
		cpu.rF.C = DoesCarry16(cpu.rSP, value);
	}
	SetNFlag(false);
	SetZFlag(false);
	cpu.rSP += value;
	return instruction_ptr + 2;
}

int Inc16Bit(unsigned char* rom, int instruction_ptr, Opcode opcode) {
	unsigned short* reg = GetRegister16(rom, instruction_ptr, opcode.opcode_name);
	*reg += 1;
	return instruction_ptr + 1;
}

int Dec16Bit(unsigned char* rom, int instruction_ptr, Opcode opcode) {
	unsigned short* reg = GetRegister16(rom, instruction_ptr, opcode.opcode_name);
	*reg -= 1;
	return instruction_ptr + 1;
}

} // namespace handlers
} // namespace back_end
