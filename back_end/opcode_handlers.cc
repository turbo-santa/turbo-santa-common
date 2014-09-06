#include "back_end/opcode_handlers.h"

namespace back_end {
namespace handlers {

unsigned char* registers = { cpu.rB, cpu.rC, cpu.rD, cpu.rE, cpu.rH, cpu.rL, cpu.rHL, cpu.rA };
unsigned char* shortRegs = { cpu.rBC, cpu.rDE, cpu.rHL, cpu.rSP, cpu.rAF };

unsigned char* GetRegister(unsigned char* rom, int instruction_ptr, unsigned char opcode_name) {
    unsigned char opcode = rom[instruction_ptr];
    unsigned char register_index = opcode - opcode_name;
    return registers + register_index;
}

unsigned char GetRegisterValue(unsigned char* rom, int instruction_ptr, unsigned char opcode_name) {
    unsigned char opcode = rom[instruction_ptr];
    unsigned char register_index = opcode - opcode_name;
    return registers[register_index];
}

unsigned char GetParameterValue(unsigned char* rom, int instruction_ptr) {
    return rom[instruction_ptr];
}

void SetZFlag(unsigned char register_value) {
    cpu.rF.Z = register_value == 0;
}

void SetNFlag(bool performed_subtraction) {
    cpu.rF.N = performed_subtraction;
}

int Add8Bit(unsigned char* rom, int instruction_ptr, Opcode opcode) {
	int index = rom[instruction_ptr] - opcode.opcode_name;
	cpu.rA += registers[index];
	return instruction_ptr + 1;
}

int Add8BitLiteral(unsigned char* rom, instruction_ptr, Opcode opcode) {
	cpu.rA += rom[instruction_ptr + 1];
	return instruction_ptr + 2;
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
    cpu.rF.Z = (*reg) == 0;
    cpu.rF.N = 0;
    cpu.rF.H = borrowed_h;
    return instruction_ptr + 1;
}

int Dec8Bit(unsigned char* rom, int instruction_ptr, Opcode opcode) {
    unsigned char* reg = GetRegister(rom, instruction_ptr, opcode.opcode_name);
    unsigned char fourth_bit = NthBit(*reg, 4);
    --(*reg);
    bool borrowed_h = forth_bit != NthBit(*reg, 4);
    cpu.rF.Z = (*reg) == 0;
    cpu.rF.N = 1;
    cpu.rF.H = borrowed_h;
    return instruction_ptr + 1;
}

} // namespace handlers
} // namespace back_end
