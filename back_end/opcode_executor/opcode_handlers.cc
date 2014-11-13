#include "back_end/opcode_executor/opcode_handlers.h"

#include "back_end/opcode_executor/opcode_executor.h"
#include "back_end/opcode_executor/opcodes.h"

namespace back_end {
namespace handlers {

using opcodes::Opcode;
using registers::cpu;

MemoryMapper* mem_map;

unsigned char* registers[] = { &cpu.bc_struct.rB, &cpu.bc_struct.rC, &cpu.de_struct.rD,
  &cpu.de_struct.rE, &cpu.hl_struct.rH, &cpu.hl_struct.rL, (unsigned char*) &cpu.rHL, &cpu.flag_struct.rA };
unsigned short shortRegs[] = { cpu.rBC, cpu.rDE, cpu.rHL, cpu.rSP, cpu.rAF };

unsigned char* GetRegister(unsigned char* rom, int instruction_ptr, unsigned char opcode_name) {
  unsigned char opcode = rom[instruction_ptr];
  unsigned char register_index = opcode - opcode_name;
  return registers[register_index];
}

unsigned short* GetRegister16(MemoryMapper* memory_mapper, int instruction_ptr, unsigned char) {
  unsigned char opcode = memory_mapper->Read(instruction_ptr);
  unsigned char register_index = (opcode >> 4);
  return shortRegs + register_index;
}

unsigned char GetRegisterValue(unsigned char* rom, int instruction_ptr, unsigned char opcode_name) {
  unsigned char opcode = rom[instruction_ptr];
  unsigned char register_index = opcode - opcode_name;
  return *registers[register_index];
}

unsigned short GetRegisterValue16(MemoryMapper* memory_mapper, int instruction_ptr, unsigned char) {
  unsigned char opcode = memory_mapper->Read(instruction_ptr);
  unsigned char register_index = (opcode >> 4);
  return shortRegs[register_index];
}

unsigned char GetParameterValue(MemoryMapper* memory_mapper, int instruction_ptr) {
  return memory_mapper->Read(instruction_ptr + 1);
}

unsigned short GetParameterValue16(MemoryMapper* memory_mapper, int instruction_ptr) {
  return (((short) memory_mapper->Read(instruction_ptr + 1)) << 8) | (short) memory_mapper->Read(instruction_ptr + 2);
}

unsigned short GetParameterValue16LS(MemoryMapper* memory_mapper, int instruction_ptr, unsigned char) {
  return (((short) memory_mapper->Read(instruction_ptr + 2)) << 8) | (short) memory_mapper->Read(instruction_ptr + 1);
}

unsigned short GetAddress16(MemoryMapper* memory_mapper, int instruction_ptr) {
  unsigned char upper = GetParameterValue(memory_mapper, instruction_ptr + 1);
  unsigned char lower = GetParameterValue(memory_mapper, instruction_ptr);
  unsigned short address = upper;
  address = address << 8;
  address |= lower;
  return address;
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

// TODO(Brendan): These opcodes are not in the same order as with ALU operations.
// 8-bit loads. Consider using opcodes::Opcode to address this.
int Ld8Bit(handlers::ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  Opcode* opcode = context->opcode;
  unsigned char* reg = opcode->reg1;
  *reg = *opcode->reg2;
  return instruction_ptr + 2;
}

int Ld8BitLiteral(handlers::ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  Opcode* opcode = context->opcode;
  unsigned char* reg = opcode->reg1;
  *reg = GetParameterValue(context->memory_mapper, instruction_ptr);
  return instruction_ptr + 2;
}

int LoadToA8Bit(handlers::ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  Opcode* opcode = context->opcode;
  cpu.flag_struct.rA = *opcode->reg1;
  return instruction_ptr + 1;
}

int LoadToA8BitLiteral(handlers::ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  Opcode opcode = *context->opcode;
  cpu.flag_struct.rA = GetParameterValue(context->memory_mapper, instruction_ptr);
  return instruction_ptr + 2;
}

// See comment below about loading to an immediate.
int LoadToA16Bit(handlers::ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  Opcode opcode = *context->opcode;
  cpu.flag_struct.rA = GetRegisterValue16(context->memory_mapper, instruction_ptr, opcode.opcode_name);
  return instruction_ptr + 1;
}

int LoadToA16BitLiteral(handlers::ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  Opcode opcode = *context->opcode;
  cpu.flag_struct.rA = GetParameterValue16(context->memory_mapper, instruction_ptr);
  return instruction_ptr + 3;
}

int LoadFromA8Bit(handlers::ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  Opcode* opcode = context->opcode;
  unsigned char* reg = opcode->reg1;
  *reg = *opcode->reg2;
  return instruction_ptr + 2;
}

int LoadFromA16Bit(handlers::ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  Opcode* opcode = context->opcode;
  unsigned char* reg = opcode->reg1;
  *reg = *opcode->reg2;
  return instruction_ptr + 2;
}

// TODO(Brendan): I am pretty sure I am missing something here. It does not
// make any sense to load a value from A to an immediate.
int LoadImediateFromA16Bit(handlers::ExecutorContext* context) {
  Opcode opcode = *context->opcode;
  return -1;
}

// ALU.
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

int Add8Bit(handlers::ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  Opcode opcode = *context->opcode;
  Add8BitImpl(*opcode.reg1);
  return instruction_ptr + 1;
}

int Add8BitLiteral(handlers::ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  Opcode opcode = *context->opcode;
  Add8BitImpl(GetParameterValue(context->memory_mapper, instruction_ptr));
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

int ADC8Bit(handlers::ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  Opcode* opcode = context->opcode;
  ADC8BitImpl(*opcode->reg1);
  return instruction_ptr + 1;
}

int ADC8BitLiteral(handlers::ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  Opcode opcode = *context->opcode;
  ADC8BitImpl(GetParameterValue(context->memory_mapper, instruction_ptr));
  return instruction_ptr + 2;
}

void Sub8BitImpl(unsigned char value) {
  cpu.flag_struct.rF.C = !DoesBorrow8(cpu.flag_struct.rA, value);
  cpu.flag_struct.rF.H = !DoesHalfBorrow8(cpu.flag_struct.rA, value);
  cpu.flag_struct.rA -= value;
  SetZFlag(cpu.flag_struct.rA);
  SetNFlag(true);
}

int Sub8Bit(handlers::ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  Opcode* opcode = context->opcode;
  Sub8BitImpl(*opcode->reg1);
  return instruction_ptr + 1;
}

int Sub8BitLiteral(handlers::ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  Opcode opcode = *context->opcode;
  Sub8BitImpl(GetParameterValue(context->memory_mapper, instruction_ptr));
  return instruction_ptr + 2;
}

int And8Bit(handlers::ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  Opcode* opcode = context->opcode;
  cpu.flag_struct.rA &= *opcode->reg1;
  SetZFlag(cpu.flag_struct.rA);
  SetNFlag(false);
  cpu.flag_struct.rF.H = 0;
  cpu.flag_struct.rF.C = 0;
  return instruction_ptr + 1;
}

int And8BitLiteral(handlers::ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  Opcode opcode = *context->opcode;
  cpu.flag_struct.rA &= GetParameterValue(context->memory_mapper, instruction_ptr);
  SetZFlag(cpu.flag_struct.rA);
  SetNFlag(false);
  cpu.flag_struct.rF.H = 0;
  cpu.flag_struct.rF.C = 0;
  return instruction_ptr + 1;
}

int Or8Bit(handlers::ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  Opcode* opcode = context->opcode;
  cpu.flag_struct.rA |= *opcode->reg1;
  SetZFlag(cpu.flag_struct.rA);
  SetNFlag(false);
  cpu.flag_struct.rF.H = 0;
  cpu.flag_struct.rF.C = 0;
  return instruction_ptr + 1;
}

int Or8BitLiteral(handlers::ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  Opcode opcode = *context->opcode;
  cpu.flag_struct.rA |= GetParameterValue(context->memory_mapper, instruction_ptr);
  SetZFlag(cpu.flag_struct.rA);
  SetNFlag(false);
  cpu.flag_struct.rF.H = 0;
  cpu.flag_struct.rF.C = 0;
  return instruction_ptr + 1;
}

int Xor8Bit(handlers::ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  Opcode* opcode = context->opcode;
  cpu.flag_struct.rA ^= *opcode->reg1;
  SetZFlag(cpu.flag_struct.rA);
  SetNFlag(false);
  cpu.flag_struct.rF.H = 0;
  cpu.flag_struct.rF.C = 0;
  return instruction_ptr + 1;
}

int Xor8BitLiteral(handlers::ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  Opcode opcode = *context->opcode;
  cpu.flag_struct.rA ^= GetParameterValue(context->memory_mapper, instruction_ptr);
  SetZFlag(cpu.flag_struct.rA);
  SetNFlag(false);
  cpu.flag_struct.rF.H = 0;
  cpu.flag_struct.rF.C = 0;
  return instruction_ptr + 1;
}

int Cp8Bit(handlers::ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  Opcode* opcode = context->opcode;
  unsigned char result = cpu.flag_struct.rA - *opcode->reg1;;
  SetZFlag(result);
  SetNFlag(true); // Performed subtraction.
  cpu.flag_struct.rF.H = NthBit(cpu.flag_struct.rA, 4) != NthBit(result, 4);
  cpu.flag_struct.rF.C = NthBit(cpu.flag_struct.rA, 7) != NthBit(result, 7);
  return instruction_ptr + 1;
}

int Cp8BitLiteral(handlers::ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  Opcode opcode = *context->opcode;
  unsigned char result = cpu.flag_struct.rA - GetParameterValue(context->memory_mapper, instruction_ptr);
  SetZFlag(result);
  SetNFlag(true); // Performed subtraction.
  cpu.flag_struct.rF.H = NthBit(cpu.flag_struct.rA, 4) != NthBit(result, 4);
  cpu.flag_struct.rF.C = NthBit(cpu.flag_struct.rA, 7) != NthBit(result, 7);
  return instruction_ptr + 1;
}

int Inc8Bit(handlers::ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  Opcode opcode = *context->opcode;
  unsigned char* reg = opcode.reg1;
  unsigned char forth_bit = NthBit(*reg, 3);
  ++(*reg);
  bool borrowed_h = forth_bit != NthBit(*reg, 3);
  SetZFlag(*reg);
  SetNFlag(false);
  cpu.flag_struct.rF.H = borrowed_h;
  return instruction_ptr + 1;
}

int Dec8Bit(handlers::ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  Opcode opcode = *context->opcode;
  unsigned char* reg = opcode.reg1;
  unsigned char fourth_bit = NthBit(*reg, 4);
  --(*reg);
  bool borrowed_h = fourth_bit != NthBit(*reg, 4);
  SetZFlag(*reg);
  SetNFlag(true);
  cpu.flag_struct.rF.H = borrowed_h;
  return instruction_ptr + 1;
}

int Add16Bit(handlers::ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  Opcode opcode = *context->opcode;
  short value = GetRegisterValue16(context->memory_mapper, instruction_ptr, opcode.opcode_name);
  cpu.flag_struct.rF.H = DoesHalfCarry16(cpu.flag_struct.rA, value);
  cpu.flag_struct.rF.C = DoesCarry16(cpu.flag_struct.rA, value);
  cpu.rHL += value;
  SetNFlag(false);
  return instruction_ptr + 1;
}

int AddSPLiteral(handlers::ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  Opcode opcode = *context->opcode;
  unsigned char value = GetParameterValue(context->memory_mapper, instruction_ptr);
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

int Inc16Bit(handlers::ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  Opcode opcode = *context->opcode;
  unsigned short* reg = GetRegister16(context->memory_mapper, instruction_ptr, opcode.opcode_name);
  *reg += 1;
  return instruction_ptr + 1;
}

int Dec16Bit(handlers::ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  Opcode opcode = *context->opcode;
  unsigned short* reg = GetRegister16(context->memory_mapper, instruction_ptr, opcode.opcode_name);
  *reg -= 1;
  return instruction_ptr + 1;
}

int Swap(handlers::ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  Opcode* opcode = context->opcode;
  unsigned char* reg = opcode->reg1;
  unsigned char value = *reg;
  *reg = (value << 4) | (value >> 4);
  SetZFlag(*reg);
  SetNFlag(false);
  cpu.flag_struct.rF.H = 0;
  cpu.flag_struct.rF.C = 0;
  return instruction_ptr + 1;
}

int DAA(handlers::ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  Opcode opcode = *context->opcode;
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

int CPL(handlers::ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  Opcode opcode = *context->opcode;
  cpu.flag_struct.rA = ~cpu.flag_struct.rA;
  cpu.flag_struct.rF.H = 1;
  SetNFlag(true);
  return instruction_ptr + 1;
}

int CCF(handlers::ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  Opcode opcode = *context->opcode;
  cpu.flag_struct.rA = ~cpu.flag_struct.rA;
  cpu.flag_struct.rF.C = !cpu.flag_struct.rF.C;
  cpu.flag_struct.rF.H = 0;
  SetNFlag(false);
  return instruction_ptr + 1;
}

int SCF(handlers::ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  Opcode opcode = *context->opcode;
  cpu.flag_struct.rF.C = 1;
  cpu.flag_struct.rF.H = 0;
  SetNFlag(false);
  return instruction_ptr + 1;
}

int NOP(handlers::ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  Opcode opcode = *context->opcode;
  return instruction_ptr + 1;
}

int Halt(handlers::ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  Opcode opcode = *context->opcode;
  // TODO: We should actually halt instead of just nop
  return instruction_ptr + 1;
}

int Stop(handlers::ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  Opcode opcode = *context->opcode;
  // TODO: We should actually stop instead of just nop
  return instruction_ptr + 1;
}

int DI(handlers::ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  Opcode opcode = *context->opcode;
  // TODO: Actually disable the interrupts
  return instruction_ptr + 1;
}

int EI(handlers::ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  Opcode opcode = *context->opcode;
  // TODO: Actually enable the interrupts
  return instruction_ptr + 1;
}

int RLCA(handlers::ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  Opcode opcode = *context->opcode;
  cpu.flag_struct.rF.C = NthBit(cpu.flag_struct.rA, 7);
  cpu.flag_struct.rA = cpu.flag_struct.rA << 1;
  cpu.flag_struct.rF.H = 0;
  SetNFlag(false);
  SetZFlag(cpu.flag_struct.rA);
  return instruction_ptr + 1;
}

int RLA(handlers::ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  Opcode opcode = *context->opcode;
  cpu.flag_struct.rF.C = NthBit(cpu.flag_struct.rA, 7);
  cpu.flag_struct.rA = cpu.flag_struct.rA << 1;
  cpu.flag_struct.rA |= cpu.flag_struct.rF.C;
  cpu.flag_struct.rF.H = 0;
  SetNFlag(false);
  SetZFlag(cpu.flag_struct.rA);
  return instruction_ptr + 1;
}

int RRCA(handlers::ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  Opcode opcode = *context->opcode;
  cpu.flag_struct.rF.C = NthBit(cpu.flag_struct.rA, 0);
  cpu.flag_struct.rA = cpu.flag_struct.rA >> 1;
  cpu.flag_struct.rF.H = 0;
  SetNFlag(false);
  SetZFlag(cpu.flag_struct.rA);
  return instruction_ptr + 1;
}

int RRA(handlers::ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  Opcode opcode = *context->opcode;
  cpu.flag_struct.rF.C = NthBit(cpu.flag_struct.rA, 0);
  cpu.flag_struct.rA = cpu.flag_struct.rA >> 1;
  cpu.flag_struct.rA |= (((unsigned char) cpu.flag_struct.rF.C) << 7);
  cpu.flag_struct.rF.H = 0;
  SetNFlag(false);
  SetZFlag(cpu.flag_struct.rA);
  return instruction_ptr + 1;
}

int RLC(handlers::ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  Opcode* opcode = context->opcode;
  unsigned char* reg = opcode->reg1;
  cpu.flag_struct.rF.C = NthBit(*reg, 7);
  *reg = *reg << 1;
  cpu.flag_struct.rF.H = 0;
  SetNFlag(false);
  SetZFlag(*reg);
  return instruction_ptr + 2;
}

int RL(handlers::ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  Opcode* opcode = context->opcode;
  unsigned char* reg = opcode->reg1;
  cpu.flag_struct.rF.C = NthBit(*reg, 7);
  *reg = *reg << 1;
  *reg |= cpu.flag_struct.rF.C;
  cpu.flag_struct.rF.H = 0;
  SetNFlag(false);
  SetZFlag(*reg);
  return instruction_ptr + 2;
}

int RRC(handlers::ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  Opcode* opcode = context->opcode;
  unsigned char* reg = opcode->reg1;
  cpu.flag_struct.rF.C = NthBit(*reg, 0);
  *reg = *reg >> 1;
  cpu.flag_struct.rF.H = 0;
  SetNFlag(false);
  SetZFlag(*reg);
  return instruction_ptr + 2;
}

int RR(handlers::ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  Opcode* opcode = context->opcode;
  unsigned char* reg = opcode->reg1;
  cpu.flag_struct.rF.C = NthBit(*reg, 0);
  *reg = *reg >> 1;
  *reg |= (((unsigned char) cpu.flag_struct.rF.C) << 7);
  cpu.flag_struct.rF.H = 0;
  SetNFlag(false);
  SetZFlag(*reg);
  return instruction_ptr + 2;
}

int SLA(handlers::ExecutorContext* context) {
  return RLC(context);
}

int SRA(handlers::ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  Opcode* opcode = context->opcode;
  unsigned char* reg = opcode->reg1;
  cpu.flag_struct.rF.C = NthBit(*reg, 0);
  unsigned char msb = NthBit(*reg, 7) << 7;
  *reg = *reg >> 1;
  *reg |= msb;
  cpu.flag_struct.rF.H = 0;
  SetNFlag(false);
  SetZFlag(*reg);
  return instruction_ptr + 2;
}

int SRL(handlers::ExecutorContext* context) {
  return RRC(context);
}

int Bit(handlers::ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  Opcode* opcode = context->opcode;
  unsigned char* reg = opcode->reg1;
  unsigned char bit = NthBit(*reg, GetParameterValue(context->memory_mapper, instruction_ptr));
  cpu.flag_struct.rF.H = 1;
  SetZFlag(bit);
  SetNFlag(false);
  return instruction_ptr + 2;
}

int Set(handlers::ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  Opcode* opcode = context->opcode;
  unsigned char* reg = opcode->reg1;
  unsigned char bit = GetParameterValue(context->memory_mapper, instruction_ptr);
  *reg |= (0x1 << bit);
  return instruction_ptr + 2;
}

int Res(handlers::ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  Opcode* opcode = context->opcode;
  unsigned char* reg = opcode->reg1;
  unsigned char bit = GetParameterValue(context->memory_mapper, instruction_ptr);
  *reg &= ~(0x1 << bit);
  return instruction_ptr + 2;
}


int Jump(handlers::ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  Opcode opcode = *context->opcode;
  instruction_ptr = GetAddress16(context->memory_mapper, instruction_ptr);
  return instruction_ptr;
}

int JumpConditional(handlers::ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  Opcode opcode = *context->opcode;
  switch (opcode.opcode_name) {
    case 0xC2:
      if (!cpu.flag_struct.rF.Z) {
        return Jump(context);
      }
      return instruction_ptr + 1;
    case 0xCA:
      if (cpu.flag_struct.rF.Z) {
        return Jump(context);
      }
      return instruction_ptr + 1;
    case 0xD2:
      if (!cpu.flag_struct.rF.C) {
        return Jump(context);
      }
      return instruction_ptr + 1;
    case 0xDA:
      if (cpu.flag_struct.rF.C) {
        return Jump(context);
      }
      return instruction_ptr + 1;
  }
  return instruction_ptr + 1;
}

int JumpHL(handlers::ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  Opcode opcode = *context->opcode;
  instruction_ptr = cpu.rHL;
  return instruction_ptr;
}

int JumpRelative(handlers::ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  Opcode opcode = *context->opcode;
  instruction_ptr += GetParameterValue(context->memory_mapper, instruction_ptr);
  return instruction_ptr;
}

int JumpRelativeConditional(handlers::ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  Opcode opcode = *context->opcode;
  switch (opcode.opcode_name) {
    case 0x20:
      if (!cpu.flag_struct.rF.Z) {
        return JumpRelative(context);
      }
      return instruction_ptr + 1;
    case 0x28:
      if (cpu.flag_struct.rF.Z) {
        return JumpRelative(context);
      }
      return instruction_ptr + 1;
    case 0x30:
      if (!cpu.flag_struct.rF.C) {
        return JumpRelative(context);
      }
      return instruction_ptr + 1;
    case 0x38:
      if (cpu.flag_struct.rF.C) {
        return JumpRelative(context);
      }
  }
  return instruction_ptr + 1;
}

int Call(handlers::ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  Opcode opcode = *context->opcode;
  unsigned short address = GetAddress16(context->memory_mapper, instruction_ptr);
  cpu.rSP = instruction_ptr + 1;
  instruction_ptr = address;
  return instruction_ptr;
}

int CallConditional(handlers::ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  Opcode opcode = *context->opcode;
  switch (opcode.opcode_name) {
    case 0xC4:
      if (!cpu.flag_struct.rF.Z) {
        return Call(context);
      }
      return instruction_ptr + 1;
    case 0xCC:
      if (cpu.flag_struct.rF.Z) {
        return Call(context);
      }
      return instruction_ptr + 1;
    case 0xD4:
      if (!cpu.flag_struct.rF.C) {
        return Call(context);
      }
      return instruction_ptr + 1;
    case 0xDC:
      if (cpu.flag_struct.rF.C) {
        return Call(context);
      }
      return instruction_ptr + 1;
  }
  return instruction_ptr + 1;
}

int Restart(handlers::ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  Opcode opcode = *context->opcode;
  unsigned char accum = 0;
  switch (opcode.opcode_name) {
    case 0xFF:
      accum += 0x08;
    case 0xF7:
      accum += 0x08;
    case 0xEF:
      accum += 0x08;
    case 0xE7:
      accum += 0x08;
    case 0xDF:
      accum += 0x08;
    case 0xD7:
      accum += 0x08;
    case 0xCF:
      accum += 0x08;
    case 0xC7:
      instruction_ptr = accum;
  }
  return instruction_ptr;
}

int Return(handlers::ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  Opcode opcode = *context->opcode;
  unsigned short address = 0; // = cpu.rSP[0];
  cpu.rSP -= 2;
  instruction_ptr = address;
  return instruction_ptr;
}

int ReturnConditional(handlers::ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  Opcode opcode = *context->opcode;
  switch (opcode.opcode_name) {
    case 0xC0:
      if (!cpu.flag_struct.rF.Z) {
        return Return(context);
      }
      return instruction_ptr + 1;
    case 0xC8:
      if (cpu.flag_struct.rF.Z) {
        return Return(context);
      }
      return instruction_ptr + 1;
    case 0xD0:
      if (!cpu.flag_struct.rF.C) {
        return Return(context);
      }
      return instruction_ptr + 1;
    case 0xD8:
      if (cpu.flag_struct.rF.C) {
        return Return(context);
      }
  }
  return instruction_ptr + 1;
}

int ReturnInterrupt(handlers::ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  Opcode opcode = *context->opcode;
  // TODO(Brendan, Diego): Implement call stack.
  unsigned short address = 0; // = cpu.rSP[0];
  cpu.rSP -= 2;
  instruction_ptr = address;
  ExecutorContext new_context(context);
  new_context.instruction_ptr = &instruction_ptr;
  EI(&new_context);
  return instruction_ptr;
}

int LoadN(handlers::ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  Opcode opcode = *context->opcode;
  unsigned char value = GetParameterValue(context->memory_mapper, instruction_ptr);
  *opcode.reg1 = value;
  return instruction_ptr + 2;
}

int LoadRR(handlers::ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  Opcode opcode = *context->opcode;
  if (opcode.opcode_name == 0x36) {
    unsigned char value = GetParameterValue(context->memory_mapper, instruction_ptr);
    *opcode.reg1 = value;
    return instruction_ptr + 2;
  } else {
    *opcode.reg1 = *opcode.reg2;
    return instruction_ptr + 1;
  }
}

int LoadAN(handlers::ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  Opcode opcode = *context->opcode;
  if (opcode.opcode_name == 0xFA) {
    // TODO(Diego): What is this for?
    unsigned short address = GetParameterValue16LS(context->memory_mapper, instruction_ptr, opcode.opcode_name);
    // TODO: figure out what nn refers to.
  } else if (opcode.opcode_name == 0x3E) {
    unsigned char value = GetParameterValue(context->memory_mapper, instruction_ptr);
    cpu.flag_struct.rA = value;
    return instruction_ptr + 2;
  }

  cpu.flag_struct.rA = *opcode.reg1;
  return instruction_ptr + 1;
}


int LoadNA(handlers::ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  Opcode opcode = *context->opcode;
  if (opcode.opcode_name == 0xEA) {
    // TODO: figure out what nn refers to.
  } else {
    *opcode.reg1 = cpu.flag_struct.rA;
  }
  return instruction_ptr + 1;
}

int LoadAC(handlers::ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  Opcode opcode = *context->opcode;
  cpu.flag_struct.rA = context->memory_mapper->Read(0xFF00 + cpu.bc_struct.rC);
  return instruction_ptr + 1;
}

int LoadCA(handlers::ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  Opcode opcode = *context->opcode;
  context->memory_mapper->Write(0xFF00 + cpu.bc_struct.rC, cpu.flag_struct.rA);
  return instruction_ptr + 1;
}

int LoadDecAHL(handlers::ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  Opcode opcode = *context->opcode;
  cpu.flag_struct.rA = context->memory_mapper->Read(cpu.rHL);
  cpu.rHL--;
  return instruction_ptr + 1;
}

int LoadDecHLA(handlers::ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  Opcode opcode = *context->opcode;
  context->memory_mapper->Write(cpu.rHL, cpu.flag_struct.rA);
  cpu.rHL--;
  return instruction_ptr + 1;
}

int LoadIncAHL(handlers::ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  Opcode opcode = *context->opcode;
  cpu.flag_struct.rA = context->memory_mapper->Read(cpu.rHL);
  cpu.rHL++;
  return instruction_ptr + 1;
}

int LoadIncHLA(handlers::ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  Opcode opcode = *context->opcode;
  context->memory_mapper->Write(cpu.rHL, cpu.flag_struct.rA);
  cpu.rHL++;
  return instruction_ptr + 1;
}

int LoadHNA(handlers::ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  Opcode opcode = *context->opcode;
  MemoryMapper* memory_mapper = context->memory_mapper;
  memory_mapper->Write(0xFF00 + GetParameterValue(memory_mapper, instruction_ptr), cpu.flag_struct.rA);
  return instruction_ptr + 2;
}

int LoadHAN(handlers::ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  Opcode opcode = *context->opcode;
  MemoryMapper* memory_mapper = context->memory_mapper;
  cpu.flag_struct.rA = memory_mapper->Read(0xFF00 + GetParameterValue(memory_mapper, instruction_ptr));
  return instruction_ptr + 2;
}

int LoadNN(handlers::ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  Opcode opcode = *context->opcode;
  *opcode.reg1 = GetParameterValue16(context->memory_mapper, instruction_ptr);
  return instruction_ptr + 3;
}

int LoadSPHL(handlers::ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  Opcode opcode = *context->opcode;
  cpu.rSP = cpu.rHL;
  return instruction_ptr + 1;
}

int LoadHLSP(handlers::ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  Opcode opcode = *context->opcode;
  cpu.rHL = cpu.rSP + (signed) GetParameterValue(context->memory_mapper, instruction_ptr);
  cpu.flag_struct.rF.Z = 0;
  SetNFlag(false);

  return instruction_ptr + 2;
}

int LoadNNSP(handlers::ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  Opcode opcode = *context->opcode;
  MemoryMapper* memory_mapper = context->memory_mapper;
  memory_mapper->Write(GetParameterValue(memory_mapper, instruction_ptr), cpu.rSP);
  return instruction_ptr + 2;
}

int Push(handlers::ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  Opcode opcode = *context->opcode;
  context->memory_mapper->Write(cpu.rSP, *opcode.reg1);
  cpu.rSP -= 2;
  return instruction_ptr + 1;
}

int Pop(handlers::ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  Opcode opcode = *context->opcode;
  MemoryMapper* memory_mapper = context->memory_mapper;
  *opcode.reg1 = (((short)memory_mapper->Read(cpu.rSP)) << 8) | memory_mapper->Read(cpu.rSP + 1); 
  cpu.rSP += 2;
  return instruction_ptr + 1;
}

} // namespace handlers
} // namespace back_end
