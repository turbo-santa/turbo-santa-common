#include "back_end/opcode_handlers.h"
#include "back_end/opcodes.h"

namespace back_end {
namespace handlers {

using back_end::opcodes::Opcode;

unsigned char registers[] = { cpu.bc_struct.rB, cpu.bc_struct.rC, cpu.de_struct.rD,
    cpu.de_struct.rE, cpu.hl_struct.rH, cpu.hl_struct.rL, (unsigned char)cpu.rHL, cpu.flag_struct.rA };
unsigned short shortRegs[] = { cpu.rBC, cpu.rDE, cpu.rHL, cpu.rSP, cpu.rAF };

unsigned char* GetRegister(unsigned char* rom, int instruction_ptr, unsigned char opcode_name) {
    unsigned char opcode = rom[instruction_ptr];
    unsigned char register_index = opcode - opcode_name;
    return registers + register_index;
}

unsigned short* GetRegister16(unsigned char* rom, int instruction_ptr, unsigned char) {
   unsigned char opcode = rom[instruction_ptr];
   unsigned char register_index = (opcode >> 4);
   return shortRegs + register_index;
}

unsigned char GetRegisterValue(unsigned char* rom, int instruction_ptr, unsigned char opcode_name) {
    unsigned char opcode = rom[instruction_ptr];
    unsigned char register_index = opcode - opcode_name;
    return registers[register_index];
}

unsigned short GetRegisterValue16(unsigned char* rom, int instruction_ptr, unsigned char) {
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
    cpu.flag_struct.rF.Z = register_value == 0;
}

void SetNFlag(bool performed_subtraction) {
    cpu.flag_struct.rF.N = performed_subtraction;
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
    if (!NthBit(left, bit) && (NthBit(right, bit) || NthBit(left - right, bit))) {
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

void Add8BitImpl(unsigned char value) {
    cpu.flag_struct.rF.C = DoesCarry8(cpu.flag_struct.rA, value);
    cpu.flag_struct.rF.H = DoesHalfCarry8(cpu.flag_struct.rA, value);
    cpu.flag_struct.rA += value;
    SetZFlag(cpu.flag_struct.rA);
    SetNFlag(false);
}

int Add8Bit(unsigned char* rom, int instruction_ptr, Opcode opcode) {
    Add8BitImpl(GetRegisterValue(rom, instruction_ptr, opcode.opcode_name));
    return instruction_ptr + 1;
}

int Add8BitLiteral(unsigned char* rom, int instruction_ptr, Opcode) {
    Add8BitImpl(GetParameterValue(rom, instruction_ptr));
    return instruction_ptr + 2;
}

void ADC8BitImpl(unsigned char value) {
    char carry = cpu.flag_struct.rF.C;
    cpu.flag_struct.rF.C = DoesCarry8(cpu.flag_struct.rA, value);
    cpu.flag_struct.rF.H = DoesHalfCarry8(cpu.flag_struct.rA, value);
    cpu.flag_struct.rA += value;
    cpu.flag_struct.rF.C |= DoesCarry8(cpu.flag_struct.rA, carry);
    cpu.flag_struct.rF.H |= DoesHalfCarry8(cpu.flag_struct.rA, carry);
    cpu.flag_struct.rA += carry;
    SetZFlag(cpu.flag_struct.rA);
    SetNFlag(false);
}

int ADC8Bit(unsigned char* rom, int instruction_ptr, Opcode opcode) {
    ADC8BitImpl(GetRegisterValue(rom, instruction_ptr, opcode.opcode_name));
    return instruction_ptr + 1;
}

int ADC8BitLiteral(unsigned char* rom, int instruction_ptr, Opcode) {
    ADC8BitImpl(GetParameterValue(rom, instruction_ptr));
    return instruction_ptr + 2;
}

void Sub8BitImpl(unsigned char value) {
    cpu.flag_struct.rF.C = !DoesBorrow8(cpu.flag_struct.rA, value);
    cpu.flag_struct.rF.H = !DoesHalfBorrow8(cpu.flag_struct.rA, value);
    cpu.flag_struct.rA -= value;
    SetZFlag(cpu.flag_struct.rA);
    SetNFlag(true);
}

int Sub8Bit(unsigned char* rom, int instruction_ptr, Opcode opcode) {
    Sub8BitImpl(GetRegisterValue(rom, instruction_ptr, opcode.opcode_name));
    return instruction_ptr + 1;
}

int Sub8BitLiteral(unsigned char* rom, int instruction_ptr, Opcode) {
    Sub8BitImpl(GetParameterValue(rom, instruction_ptr));
    return instruction_ptr + 2;
}

int And8Bit(unsigned char* rom, int instruction_ptr, Opcode opcode) {
    cpu.flag_struct.rA &= GetRegisterValue(rom, instruction_ptr, opcode.opcode_name);
    SetZFlag(cpu.flag_struct.rA);
    SetNFlag(false);
    cpu.flag_struct.rF.H = 0;
    cpu.flag_struct.rF.C = 0;
    return instruction_ptr + 1;
}

int And8BitLiteral(unsigned char* rom, int instruction_ptr, Opcode) {
    cpu.flag_struct.rA &= GetParameterValue(rom, instruction_ptr);
    SetZFlag(cpu.flag_struct.rA);
    SetNFlag(false);
    cpu.flag_struct.rF.H = 0;
    cpu.flag_struct.rF.C = 0;
    return instruction_ptr + 1;
}

int Or8Bit(unsigned char* rom, int instruction_ptr, Opcode opcode) {
    cpu.flag_struct.rA |= GetRegisterValue(rom, instruction_ptr, opcode.opcode_name);
    SetZFlag(cpu.flag_struct.rA);
    SetNFlag(false);
    cpu.flag_struct.rF.H = 0;
    cpu.flag_struct.rF.C = 0;
    return instruction_ptr + 1;
}

int Or8BitLiteral(unsigned char* rom, int instruction_ptr, Opcode) {
    cpu.flag_struct.rA |= GetParameterValue(rom, instruction_ptr);
    SetZFlag(cpu.flag_struct.rA);
    SetNFlag(false);
    cpu.flag_struct.rF.H = 0;
    cpu.flag_struct.rF.C = 0;
    return instruction_ptr + 1;
}

int Xor8Bit(unsigned char* rom, int instruction_ptr, Opcode opcode) {
    cpu.flag_struct.rA ^= GetRegisterValue(rom, instruction_ptr, opcode.opcode_name);
    SetZFlag(cpu.flag_struct.rA);
    SetNFlag(false);
    cpu.flag_struct.rF.H = 0;
    cpu.flag_struct.rF.C = 0;
    return instruction_ptr + 1;
}

int Xor8BitLiteral(unsigned char* rom, int instruction_ptr, Opcode) {
    cpu.flag_struct.rA ^= GetParameterValue(rom, instruction_ptr);
    SetZFlag(cpu.flag_struct.rA);
    SetNFlag(false);
    cpu.flag_struct.rF.H = 0;
    cpu.flag_struct.rF.C = 0;
    return instruction_ptr + 1;
}

int Cp8Bit(unsigned char* rom, int instruction_ptr, Opcode opcode) {
    unsigned char result = cpu.flag_struct.rA - GetRegisterValue(rom, instruction_ptr, opcode.opcode_name);
    SetZFlag(result);
    SetNFlag(true); // Performed subtraction.
    cpu.flag_struct.rF.H = NthBit(cpu.flag_struct.rA, 4) != NthBit(result, 4);
    cpu.flag_struct.rF.C = NthBit(cpu.flag_struct.rA, 7) != NthBit(result, 7);
    return instruction_ptr + 1;
}

int Cp8BitLiteral(unsigned char* rom, int instruction_ptr, Opcode) {
    unsigned char result = cpu.flag_struct.rA - GetParameterValue(rom, instruction_ptr);
    SetZFlag(result);
    SetNFlag(true); // Performed subtraction.
    cpu.flag_struct.rF.H = NthBit(cpu.flag_struct.rA, 4) != NthBit(result, 4);
    cpu.flag_struct.rF.C = NthBit(cpu.flag_struct.rA, 7) != NthBit(result, 7);
    return instruction_ptr + 1;
}

int Inc8Bit(unsigned char* rom, int instruction_ptr, Opcode opcode) {
    unsigned char* reg = GetRegister(rom, instruction_ptr, opcode.opcode_name);
    unsigned char forth_bit = NthBit(*reg, 3);
    ++(*reg);
    bool borrowed_h = forth_bit != NthBit(*reg, 3);
    SetZFlag(*reg);
    SetNFlag(false);
    cpu.flag_struct.rF.H = borrowed_h;
    return instruction_ptr + 1;
}

int Dec8Bit(unsigned char* rom, int instruction_ptr, Opcode opcode) {
    unsigned char* reg = GetRegister(rom, instruction_ptr, opcode.opcode_name);
    unsigned char fourth_bit = NthBit(*reg, 4);
    --(*reg);
    bool borrowed_h = fourth_bit != NthBit(*reg, 4);
    SetZFlag(*reg);
    SetNFlag(true);
    cpu.flag_struct.rF.H = borrowed_h;
    return instruction_ptr + 1;
}

int Add16Bit(unsigned char* rom, int instruction_ptr, Opcode opcode) {
    short value = GetRegisterValue16(rom, instruction_ptr, opcode.opcode_name);
    cpu.flag_struct.rF.H = DoesHalfCarry16(cpu.flag_struct.rA, value);
    cpu.flag_struct.rF.C = DoesCarry16(cpu.flag_struct.rA, value);
    cpu.rHL += value;
    SetNFlag(false);
    return instruction_ptr + 1;
}

int AddSPLiteral(unsigned char* rom, int instruction_ptr, Opcode) {
    unsigned char value = GetParameterValue(rom, instruction_ptr);
    if (NthBit(value, 7)) {
        cpu.flag_struct.rF.H = DoesHalfBorrow16(cpu.rSP, value);
        cpu.flag_struct.rF.C = DoesBorrow16(cpu.rSP, value);
    } else {
        cpu.flag_struct.rF.H = DoesHalfCarry16(cpu.rSP, value);
        cpu.flag_struct.rF.C = DoesCarry16(cpu.rSP, value);
    }
    SetNFlag(false);
    cpu.flag_struct.rF.Z = 0;
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

int Swap(unsigned char* rom, int instruction_ptr, Opcode opcode) {
    unsigned char* reg = GetRegister(rom, instruction_ptr, opcode.opcode_name);
    unsigned char value = *reg;
    *reg = (value << 4) | (value >> 4);
    SetZFlag(*reg);
    SetNFlag(false);
    cpu.flag_struct.rF.H = 0;
    cpu.flag_struct.rF.C = 0;
    return instruction_ptr + 1;
}

int DAA(unsigned char* rom, int instruction_ptr, Opcode opcode) {
    unsigned char upper = cpu.flag_struct.rA >> 4;
    unsigned char lower = cpu.flag_struct.rA & 0x0F;
    unsigned char c = cpu.flag_struct.rF.C;
    unsigned char h = cpu.flag_struct.rF.H;
    if ((!c && upper < 0x9  && lower > 0x9 && !h) || (!c && upper < 0xA && lower < 0x4 && h)) {
        cpu.flag_struct.rA += 0x06;
    } else if ((!c && upper > 0x9 && lower < 0xA && !h) || (c && upper < 0x3 && lower < 0xA && !h)) {
        cpu.flag_struct.rA += 0x60;
        cpu.flag_struct.rF.C = 1;
    } else if ((!c && upper > 0x8 && lower > 0x9 && !h) || (!c && upper > 0x9 && lower < 0x4 && h)
        || (c && upper < 0x3 && lower > 0x9 && !h) || (c && upper < 0x4 && lower < 0x4 && h)) {
        cpu.flag_struct.rA += 0x66;
        cpu.flag_struct.rF.C = 1;
    } else if (!c && upper < 0x9 && lower > 0x5 && h) {
        cpu.flag_struct.rA += 0xFA;
    } else if (c && upper > 0x6 && lower < 0xA && !h) {
        cpu.flag_struct.rA += 0xA0;
    } else if (c && upper > 0x5 && lower > 0x5 && h) {
        cpu.flag_struct.rA += 0x9A;
    }
    cpu.flag_struct.rF.H = 0;
    SetZFlag(cpu.flag_struct.rA);
    return instruction_ptr + 1;
}

int CPL(unsigned char* rom, int instruction_ptr, Opcode opcode) {
    cpu.flag_struct.rA ~= cpu.flag_struct.rA;
    cpu.flag_struct.rF.H = 1;
    SetNFlag(true);
    return instruction_ptr + 1;
}

int CCF(unsigned char* rom, int instruction_ptr, Opcode opcode) {
    cpu.flag_struct.rA ~= cpu.flag_struct.rA;
    cpu.flag_struct.rF.C != cpu.flag_struct.rF.C;
    cpu.flag_struct.rF.H = 0;
    SetNFlag(false);
    return instruction_ptr + 1;
}

int SCF(unsigned char* rom, int instruction_ptr, Opcode opcode) {
    cpu.flag_struct.rF.C = 1;
    cpu.flag_struct.rF.H = 0;
    SetNFlag(false);
    return instruction_ptr + 1;
}

int NOP(unsigned char* rom, int instruction_ptr, Opcode opcode) {
    return instruction_ptr + 1;
}

int Halt(unsigned char* rom, int instruction_ptr, Opcode opcode) {
    // TODO: We should actually halt instead of just nop
    return instruction_ptr + 1;
}

int Stop(unsigned char* rom, int instruction_ptr, Opcode opcode) {
    // TODO: We should actually stop instead of just nop
    return instruction_ptr + 1;
}

int DI(unsigned char* rom, int instruction_ptr, Opcode opcode) {
    // TODO: Actually disable the interrupts
    return instruction_ptr + 1;
}

int EI(unsigned char* rom, int instruction_ptr, Opcode opcode) {
    // TODO: Actually enable the interrupts
    return instruction_ptr + 1;
}

} // namespace handlers
} // namespace back_end
