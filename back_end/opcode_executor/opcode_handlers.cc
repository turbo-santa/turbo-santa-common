#include "back_end/config.h"

#include "back_end/opcode_executor/opcode_handlers.h"
#include "back_end/opcode_executor/opcode_executor.h"
#include "back_end/opcode_executor/opcodes.h"
#include <glog/logging.h>
#include <ncurses.h>

namespace back_end {
namespace handlers {

using opcodes::Opcode;
using registers::GB_CPU;
using memory::MemoryMapper;
using std::string;

unsigned char GetParameterValue(MemoryMapper* memory_mapper, int instruction_ptr) {
  return memory_mapper->Read(instruction_ptr);
}

unsigned short GetParameterValue16(MemoryMapper* memory_mapper, int instruction_ptr) {
  return (((short) memory_mapper->Read(instruction_ptr + 1)) << 8) | (short) memory_mapper->Read(instruction_ptr);
}

unsigned short GetAddress16(MemoryMapper* memory_mapper, int instruction_ptr) {
  unsigned char upper = GetParameterValue(memory_mapper, instruction_ptr + 1);
  unsigned char lower = GetParameterValue(memory_mapper, instruction_ptr);
  unsigned short address = upper;
  address = address << 8;
  address |= lower;
  return address;
}

void SetZFlag(unsigned char register_value, GB_CPU* cpu) {
  cpu->flag_struct.rF.Z = register_value == 0;
}

void SetNFlag(bool performed_subtraction, GB_CPU* cpu) {
  cpu->flag_struct.rF.N = performed_subtraction;
}

unsigned char NthBit(unsigned int byte, int n) {
  return (byte >> n) & 1;
}

string Hex(unsigned short i) {
  std::stringstream stream;
  stream << "0x" << std::hex << i;
  return stream.str();
}
  
string RegisterName8(void* reg, GB_CPU* cpu) {
  if (reg == &cpu->flag_struct.rA) {
      return "A";
  } else if (reg == &cpu->bc_struct.rB) {
      return "B";
  } else if (reg == &cpu->bc_struct.rC) {
      return "C";
  } else if (reg == &cpu->de_struct.rD) {
      return "D";
  } else if (reg == &cpu->de_struct.rE) {
      return "E";
  } else if (reg == &cpu->flag_struct.rF) {
      return "F";
  } else if (reg == &cpu->hl_struct.rL) {
      return "L";
  } else if (reg == &cpu->hl_struct.rH) {
      return "H";
  } else {
    return "ERROR GETTING REGISTER NAME!";
  }
}
  
string RegisterName16(void* reg, GB_CPU* cpu) {
  if (reg == &cpu->rAF) {
    return "AF";
  } else if (reg == &cpu->rBC) {
    return "BC";
  } else if (reg == &cpu->rDE) {
    return "DE";
  } else if (reg == &cpu->rHL) {
    return "HL";
  } else if (reg == &cpu->rSP) {
    return "SP";
  } else if (reg == &cpu->rPC) {
    return "PC";
  } else {
    return "ERROR GETTING REGISTER NAME!";
  }
}

void PrintInstruction(string instruction) {
  LOG(INFO) << "Instruction: " << instruction;
}

void PrintInstruction(string instruction, string arg1) {
  LOG(INFO) << "Instruction: " << instruction << " " << arg1;
}

void PrintInstruction(string instruction, string arg1, string arg2) {
  LOG(INFO) << "Instruction: " << instruction << " " << arg1 << "," << arg2;
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
  // Cases:
  //  01110110  01110010  00011011
  // -00001111 -00000100 -00001100
  //  01100111  01101110  00001111
  
  if ((!NthBit(left, bit) && (NthBit(right, bit) || NthBit(left - right, bit))) || (NthBit(left, bit) && NthBit(right, bit) && NthBit(left - right, bit))) {
    return true;
  }
  return false;
}

bool DoesBorrow8(unsigned char left, unsigned char right) {
  return DoesUnderflow(left, right, 7);
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

void Add8BitImpl(unsigned char value, GB_CPU* cpu) {
  cpu->flag_struct.rF.C = DoesCarry8(cpu->flag_struct.rA, value);
  cpu->flag_struct.rF.H = DoesHalfCarry8(cpu->flag_struct.rA, value);
  cpu->flag_struct.rA += value;
  SetZFlag(cpu->flag_struct.rA, cpu);
  SetNFlag(false, cpu);
}

int Add8Bit(handlers::ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  Opcode opcode = *context->opcode;
  Add8BitImpl(*opcode.reg1, context->cpu);
  PrintInstruction("ADD", "A", RegisterName8(opcode.reg1, context->cpu));
  return instruction_ptr;
}

int Add8BitAddress(handlers::ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  Opcode opcode = *context->opcode;
  Add8BitImpl(context->memory_mapper->Read(*opcode.reg1), context->cpu);
  PrintInstruction("ADD", "A", "(" + RegisterName16(opcode.reg1, context->cpu) + ")");
  return instruction_ptr;
}

int Add8BitLiteral(handlers::ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  Opcode opcode = *context->opcode;
  Add8BitImpl(GetParameterValue(context->memory_mapper, instruction_ptr), context->cpu);
  PrintInstruction("ADD", "A", Hex(GetParameterValue(context->memory_mapper, instruction_ptr)));
  return instruction_ptr + 1;
}

void ADC8BitImpl(unsigned char value, GB_CPU* cpu) {
  char carry = cpu->flag_struct.rF.C;
  cpu->flag_struct.rF.C = DoesCarry8(cpu->flag_struct.rA, value + carry);
  cpu->flag_struct.rF.H = DoesHalfCarry8(cpu->flag_struct.rA, value + carry);
  cpu->flag_struct.rA += value;
  cpu->flag_struct.rA += carry;
  SetZFlag(cpu->flag_struct.rA, cpu);
  SetNFlag(false, cpu);
}

int ADC8Bit(handlers::ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  Opcode* opcode = context->opcode;
  ADC8BitImpl(*opcode->reg1, context->cpu);
  PrintInstruction("ADC", "A", RegisterName8(opcode->reg1, context->cpu));
  return instruction_ptr;
}

int ADC8BitAddress(handlers::ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  Opcode* opcode = context->opcode;
  ADC8BitImpl(context->memory_mapper->Read(*opcode->reg1), context->cpu);
  PrintInstruction("ADC", "A", "(" + RegisterName16(opcode->reg1, context->cpu) + ")");
  return instruction_ptr;
}

int ADC8BitLiteral(handlers::ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  Opcode opcode = *context->opcode;
  ADC8BitImpl(GetParameterValue(context->memory_mapper, instruction_ptr), context->cpu);
  PrintInstruction("ADC", "A", Hex(GetParameterValue(context->memory_mapper, instruction_ptr)));
  return instruction_ptr + 1;
}

void Sub8BitImpl(unsigned char value, GB_CPU* cpu) {
  cpu->flag_struct.rF.C = !DoesBorrow8(cpu->flag_struct.rA, value);
  cpu->flag_struct.rF.H = !DoesHalfBorrow8(cpu->flag_struct.rA, value);
  cpu->flag_struct.rA -= value;
  SetZFlag(cpu->flag_struct.rA, cpu);
  SetNFlag(true, cpu);
}

int Sub8Bit(handlers::ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  Opcode* opcode = context->opcode;
  Sub8BitImpl(*opcode->reg1, context->cpu);
  PrintInstruction("SUB", "A", RegisterName8(opcode->reg1, context->cpu));
  return instruction_ptr;
}

int Sub8BitAddress(handlers::ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  Opcode* opcode = context->opcode;
  Sub8BitImpl(context->memory_mapper->Read(*opcode->reg1), context->cpu);
  PrintInstruction("SUB", "A", "(" + RegisterName16(opcode->reg1, context->cpu) + ")");
  return instruction_ptr;
}
  
int Sub8BitLiteral(handlers::ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  Opcode opcode = *context->opcode;
  Sub8BitImpl(GetParameterValue(context->memory_mapper, instruction_ptr), context->cpu);
  PrintInstruction("SUB", "A", Hex(GetParameterValue(context->memory_mapper, instruction_ptr)));
  return instruction_ptr + 1;
}

void SBC8BitImpl(unsigned char value, GB_CPU* cpu) {
    char carry = cpu->flag_struct.rF.C;
    cpu->flag_struct.rF.C = !DoesBorrow8(cpu->flag_struct.rA, value + carry);
    cpu->flag_struct.rF.H = !DoesHalfBorrow8(cpu->flag_struct.rA, value + carry);
    cpu->flag_struct.rA -= value;
    cpu->flag_struct.rA -= carry;
    SetZFlag(cpu->flag_struct.rA, cpu);
    SetNFlag(true, cpu);
}

int SBC8Bit(handlers::ExecutorContext* context) {
    SBC8BitImpl(*context->opcode->reg1, context->cpu);
    PrintInstruction("SBC", "A", RegisterName8(context->opcode->reg1, context->cpu));
    return *context->instruction_ptr;
}

int SBC8BitAddress(handlers::ExecutorContext* context) {
  SBC8BitImpl(context->memory_mapper->Read(*context->opcode->reg1), context->cpu);
  PrintInstruction("SBC", "A", "(" + RegisterName16(context->opcode->reg1, context->cpu) + ")");
  return *context->instruction_ptr;
}

int SBC8BitLiteral(handlers::ExecutorContext* context) {
    SBC8BitImpl(GetParameterValue(context->memory_mapper, *context->instruction_ptr), context->cpu);
    PrintInstruction("SBC", "A", Hex(GetParameterValue(context->memory_mapper, *context->instruction_ptr)));
    return *context->instruction_ptr;
}

int And8Bit(handlers::ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  Opcode* opcode = context->opcode;
  context->cpu->flag_struct.rA &= *opcode->reg1;
  SetZFlag(context->cpu->flag_struct.rA, context->cpu);
  SetNFlag(false, context->cpu);
  context->cpu->flag_struct.rF.H = 1;
  context->cpu->flag_struct.rF.C = 0;
  PrintInstruction("AND", "A", RegisterName8(opcode->reg1, context->cpu));
  return instruction_ptr;
}
  
int And8BitAddress(handlers::ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  Opcode* opcode = context->opcode;
  context->cpu->flag_struct.rA &= context->memory_mapper->Read(*opcode->reg1);
  SetZFlag(context->cpu->flag_struct.rA, context->cpu);
  SetNFlag(false, context->cpu);
  context->cpu->flag_struct.rF.H = 1;
  context->cpu->flag_struct.rF.C = 0;
  PrintInstruction("AND", "A", "(" + RegisterName16(opcode->reg1, context->cpu) + ")");
  return instruction_ptr;
}
  
int And8BitLiteral(handlers::ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  Opcode opcode = *context->opcode;
  context->cpu->flag_struct.rA &= GetParameterValue(context->memory_mapper, instruction_ptr);
  SetZFlag(context->cpu->flag_struct.rA, context->cpu);
  SetNFlag(false, context->cpu);
  context->cpu->flag_struct.rF.H = 1;
  context->cpu->flag_struct.rF.C = 0;
  PrintInstruction("AND", "A", Hex(GetParameterValue(context->memory_mapper, instruction_ptr)));
  return instruction_ptr + 1;
}

int Or8Bit(handlers::ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  Opcode* opcode = context->opcode;
  context->cpu->flag_struct.rA |= *opcode->reg1;
  SetZFlag(context->cpu->flag_struct.rA, context->cpu);
  SetNFlag(false, context->cpu);
  context->cpu->flag_struct.rF.H = 0;
  context->cpu->flag_struct.rF.C = 0;
  PrintInstruction("OR", "A", RegisterName8(opcode->reg1, context->cpu));
  return instruction_ptr;
}

int Or8BitAddress(handlers::ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  Opcode* opcode = context->opcode;
  context->cpu->flag_struct.rA |= context->memory_mapper->Read(*opcode->reg1);
  SetZFlag(context->cpu->flag_struct.rA, context->cpu);
  SetNFlag(false, context->cpu);
  context->cpu->flag_struct.rF.H = 0;
  context->cpu->flag_struct.rF.C = 0;
  PrintInstruction("OR", "A", "(" + RegisterName16(opcode->reg1, context->cpu) + ")");
  return instruction_ptr;
}
  
int Or8BitLiteral(handlers::ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  Opcode opcode = *context->opcode;
  context->cpu->flag_struct.rA |= GetParameterValue(context->memory_mapper, instruction_ptr);
  SetZFlag(context->cpu->flag_struct.rA, context->cpu);
  SetNFlag(false, context->cpu);
  context->cpu->flag_struct.rF.H = 0;
  context->cpu->flag_struct.rF.C = 0;
  PrintInstruction("OR", "A", Hex(GetParameterValue(context->memory_mapper, instruction_ptr)));
  return instruction_ptr + 1;
}

int Xor8Bit(handlers::ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  Opcode* opcode = context->opcode;
  context->cpu->flag_struct.rA ^= *opcode->reg1;
  SetZFlag(context->cpu->flag_struct.rA, context->cpu);
  SetNFlag(false, context->cpu);
  context->cpu->flag_struct.rF.H = 0;
  context->cpu->flag_struct.rF.C = 0;
  PrintInstruction("XOR", "A", RegisterName8(opcode->reg1, context->cpu));
  return instruction_ptr;
}

int Xor8BitAddress(handlers::ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  Opcode* opcode = context->opcode;
  context->cpu->flag_struct.rA ^= context->memory_mapper->Read(*opcode->reg1);
  SetZFlag(context->cpu->flag_struct.rA, context->cpu);
  SetNFlag(false, context->cpu);
  context->cpu->flag_struct.rF.H = 0;
  context->cpu->flag_struct.rF.C = 0;
  PrintInstruction("XOR", "A", "(" + RegisterName16(opcode->reg1, context->cpu) + ")");
  return instruction_ptr;
}

int Xor8BitLiteral(handlers::ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  Opcode opcode = *context->opcode;
  context->cpu->flag_struct.rA ^= GetParameterValue(context->memory_mapper, instruction_ptr);
  SetZFlag(context->cpu->flag_struct.rA, context->cpu);
  SetNFlag(false, context->cpu);
  context->cpu->flag_struct.rF.H = 0;
  context->cpu->flag_struct.rF.C = 0;
  PrintInstruction("XOR", "A", Hex(GetParameterValue(context->memory_mapper, instruction_ptr)));
  return instruction_ptr + 1;
}

int Cp8Bit(handlers::ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  Opcode* opcode = context->opcode;
  unsigned char result = context->cpu->flag_struct.rA - *opcode->reg1;
  SetZFlag(result, context->cpu);
  SetNFlag(true, context->cpu); // Performed subtraction.
  context->cpu->flag_struct.rF.H = !DoesHalfBorrow8(context->cpu->flag_struct.rA, *opcode->reg1);
  context->cpu->flag_struct.rF.C = !DoesBorrow8(context->cpu->flag_struct.rA, *opcode->reg1);
  PrintInstruction("CP", "A", RegisterName8(opcode->reg1, context->cpu));
  LOG(INFO) << "Z flag = " << 0x0000 + context->cpu->flag_struct.rF.Z;
  LOG(INFO) << "N flag = " << 0x0000 + context->cpu->flag_struct.rF.N;
  LOG(INFO) << "H flag = " << 0x0000 + context->cpu->flag_struct.rF.H;
  LOG(INFO) << "C flag = " << 0x0000 + context->cpu->flag_struct.rF.C;
  return instruction_ptr;
}

int Cp8BitAddress(handlers::ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  Opcode* opcode = context->opcode;
  unsigned char value = context->memory_mapper->Read(*opcode->reg1);
  unsigned char result = context->cpu->flag_struct.rA - value;
  SetZFlag(result, context->cpu);
  SetNFlag(true, context->cpu); // Performed subtraction.
  context->cpu->flag_struct.rF.H = !DoesHalfBorrow8(context->cpu->flag_struct.rA, value);
  context->cpu->flag_struct.rF.C = !DoesBorrow8(context->cpu->flag_struct.rA, value);
  PrintInstruction("CP", "A", "(" + RegisterName16(opcode->reg1, context->cpu) + ")");
  LOG(INFO) << "Z flag = " << 0x0000 + context->cpu->flag_struct.rF.Z;
  LOG(INFO) << "N flag = " << 0x0000 + context->cpu->flag_struct.rF.N;
  LOG(INFO) << "H flag = " << 0x0000 + context->cpu->flag_struct.rF.H;
  LOG(INFO) << "C flag = " << 0x0000 + context->cpu->flag_struct.rF.C;
  return instruction_ptr;
}
  
  
int Cp8BitLiteral(handlers::ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  Opcode opcode = *context->opcode;
  unsigned char value = GetParameterValue(context->memory_mapper, instruction_ptr);
  unsigned char result = context->cpu->flag_struct.rA - value;
  SetZFlag(result, context->cpu);
  SetNFlag(true, context->cpu); // Performed subtraction.
  context->cpu->flag_struct.rF.H = !DoesHalfBorrow8(context->cpu->flag_struct.rA, value);
  context->cpu->flag_struct.rF.C = !DoesBorrow8(context->cpu->flag_struct.rA, value);
  PrintInstruction("CP", "A", Hex(GetParameterValue(context->memory_mapper, instruction_ptr)));
  LOG(INFO) << "Z flag = " << 0x0000 + context->cpu->flag_struct.rF.Z;
  LOG(INFO) << "N flag = " << 0x0000 + context->cpu->flag_struct.rF.N;
  LOG(INFO) << "H flag = " << 0x0000 + context->cpu->flag_struct.rF.H;
  LOG(INFO) << "C flag = " << 0x0000 + context->cpu->flag_struct.rF.C;
  return instruction_ptr + 1;
}

int Inc8Bit(handlers::ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  Opcode opcode = *context->opcode;
  unsigned char* reg = (unsigned char*) opcode.reg1;
  unsigned char fourth_bit = NthBit(*reg, 3);
  bool half_carry = DoesHalfCarry8(*reg, 1);
  ++(*reg);
  
  SetZFlag(*reg, context->cpu);
  SetNFlag(false, context->cpu);
  context->cpu->flag_struct.rF.H = half_carry;
  PrintInstruction("INC", RegisterName8(opcode.reg1, context->cpu));
  return instruction_ptr;
}

int Inc8BitAddress(handlers::ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  Opcode opcode = *context->opcode;
  unsigned char val = context->memory_mapper->Read(*opcode.reg1);
  bool half_carry = DoesHalfCarry8(val, 1);
  context->memory_mapper->Write(*opcode.reg1, ++val);

  SetZFlag(val, context->cpu);
  SetNFlag(false, context->cpu);
  context->cpu->flag_struct.rF.H = half_carry;
  PrintInstruction("INC", "(" + RegisterName16(opcode.reg1, context->cpu) + ")");
  return instruction_ptr;
}
  
int Dec8Bit(handlers::ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  Opcode opcode = *context->opcode;
  unsigned char* reg = (unsigned char*) opcode.reg1;
  bool borrowed_h = DoesHalfBorrow8(*reg, 1);
  --(*reg);
  
  SetZFlag(*reg, context->cpu);
  SetNFlag(true, context->cpu);
  context->cpu->flag_struct.rF.H = borrowed_h;
  if (opcode.opcode_name == 0x05) {
    LOG(INFO) << "B decremented to " << std::hex << 0x0000 + *reg;
    LOG(INFO) << "Z flag is " << std::dec << 0x0000 + context->cpu->flag_struct.rF.Z;
  }
  PrintInstruction("DEC", RegisterName8(opcode.reg1, context->cpu));
  return instruction_ptr;
}

int Dec8BitAddress(handlers::ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  Opcode opcode = *context->opcode;
  unsigned char val = context->memory_mapper->Read(*opcode.reg1);
  bool borrowed_h = DoesHalfBorrow8(val, 1);
  context->memory_mapper->Write(*opcode.reg1, --val);
  
  SetZFlag(val, context->cpu);
  SetNFlag(true, context->cpu);
  context->cpu->flag_struct.rF.H = borrowed_h;
  PrintInstruction("DEC", "(" + RegisterName16(opcode.reg1, context->cpu) + ")");
  return instruction_ptr;
}
  
int Add16Bit(handlers::ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  Opcode opcode = *context->opcode;
  context->cpu->flag_struct.rF.H = DoesHalfCarry16(context->cpu->rHL, *opcode.reg1);
  context->cpu->flag_struct.rF.C = DoesCarry16(context->cpu->rHL, *opcode.reg1);
  context->cpu->rHL += *opcode.reg1;
  SetNFlag(false, context->cpu);
  PrintInstruction("ADD", "HL", RegisterName16(opcode.reg1, context->cpu));
  return instruction_ptr;
}

int AddSPLiteral(handlers::ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  Opcode opcode = *context->opcode;
  // This value is signed.
  char value = static_cast<char>(GetParameterValue(context->memory_mapper, instruction_ptr));
  if (NthBit(value, 7)) {
    // TODO(Brendan): Make sure this works the same for signed.
    context->cpu->flag_struct.rF.H = DoesHalfBorrow16(context->cpu->rSP, value);
    context->cpu->flag_struct.rF.C = DoesBorrow16(context->cpu->rSP, value);
  } else {
    context->cpu->flag_struct.rF.H = DoesHalfCarry16(context->cpu->rSP, value);
    context->cpu->flag_struct.rF.C = DoesCarry16(context->cpu->rSP, value);
  }
  SetNFlag(false, context->cpu);
  context->cpu->flag_struct.rF.Z = 0;
  context->cpu->rSP += value;
  PrintInstruction("ADD", "SP", Hex(GetParameterValue(context->memory_mapper, instruction_ptr)));
  return instruction_ptr + 1;
}

int Inc16Bit(handlers::ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  Opcode opcode = *context->opcode;
  unsigned short* reg = (unsigned short*) opcode.reg1;
  ++(*opcode.reg1);
  // No flags are affected by this instruction
  PrintInstruction("INC", RegisterName16(opcode.reg1, context->cpu));
  return instruction_ptr;
}

int Dec16Bit(handlers::ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  Opcode opcode = *context->opcode;
  unsigned short* reg = (unsigned short*) opcode.reg1;
  --(*opcode.reg1);
  // No flags are affected by this instruction
  PrintInstruction("DEC", RegisterName16(opcode.reg1, context->cpu));
  return instruction_ptr;
}

int Swap(handlers::ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  Opcode* opcode = context->opcode;
  unsigned char* reg = (unsigned char*) opcode->reg1;
  unsigned char value = *reg;
  *reg = (value << 4) | (value >> 4);
  SetZFlag(*reg, context->cpu);
  SetNFlag(false, context->cpu);
  context->cpu->flag_struct.rF.H = 0;
  context->cpu->flag_struct.rF.C = 0;
  PrintInstruction("SWAP", RegisterName8(opcode->reg1, context->cpu));
  return instruction_ptr;
}
  
int SwapAddress(handlers::ExecutorContext* context) {
  unsigned char val = context->memory_mapper->Read(*context->opcode->reg1);
  unsigned char swapVal = (val << 4) | (val >> 4);
  context->memory_mapper->Write(*context->opcode->reg1, swapVal);
  SetZFlag(swapVal, context->cpu);
  SetNFlag(false, context->cpu);
  context->cpu->flag_struct.rF.H = 0;
  context->cpu->flag_struct.rF.C = 0;
  PrintInstruction("SWAP", "(" + RegisterName16(context->opcode->reg1, context->cpu) + ")");
  return *context->instruction_ptr;
}

int DAA(handlers::ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  Opcode opcode = *context->opcode;

  unsigned char mul = 0x6;
  unsigned char sum = context->cpu->flag_struct.rA;
  unsigned char n = sum / 10;
  while (n > 0) {
    sum += n * mul;
    n /= 10;
    mul *= 0x0F;
  }
  context->cpu->flag_struct.rF.C = context->cpu->flag_struct.rA > 0x64;
  context->cpu->flag_struct.rA = sum;
  context->cpu->flag_struct.rF.H = 0;
  SetZFlag(context->cpu->flag_struct.rA, context->cpu);
  PrintInstruction("DAA");
  return instruction_ptr;
}

int CPL(handlers::ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  Opcode opcode = *context->opcode;
  context->cpu->flag_struct.rA = ~context->cpu->flag_struct.rA;
  context->cpu->flag_struct.rF.H = 1;
  SetNFlag(true, context->cpu);
  PrintInstruction("CPL");
  return instruction_ptr;
}

int CCF(handlers::ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  Opcode opcode = *context->opcode;
  context->cpu->flag_struct.rA = ~context->cpu->flag_struct.rA;
  context->cpu->flag_struct.rF.C = !context->cpu->flag_struct.rF.C;
  context->cpu->flag_struct.rF.H = 0;
  SetNFlag(false, context->cpu);
  PrintInstruction("CCF");
  return instruction_ptr;
}

int SCF(handlers::ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  Opcode opcode = *context->opcode;
  context->cpu->flag_struct.rF.C = 1;
  context->cpu->flag_struct.rF.H = 0;
  SetNFlag(false, context->cpu);
  PrintInstruction("SCF");
  return instruction_ptr;
}

int NOP(handlers::ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  Opcode opcode = *context->opcode;
  PrintInstruction("NOP");
  return instruction_ptr;
}

int Halt(handlers::ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  Opcode opcode = *context->opcode;
  LOG(WARNING) << "UNINPLEMENTED OPCODE: Halt";
  // TODO: We should actually halt instead of just nop
  PrintInstruction("HALT");
  return instruction_ptr;
}

int Stop(handlers::ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  Opcode opcode = *context->opcode;
  LOG(WARNING) << "UNINPLEMENTED OPCODE: Stop";
  // TODO: We should actually stop instead of just nop
  PrintInstruction("STOP");
  return instruction_ptr;
}

int DI(handlers::ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  Opcode opcode = *context->opcode;
  *context->interrupt_master_enable = false;
  PrintInstruction("DI");
  return instruction_ptr;
}

int EI(handlers::ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  Opcode opcode = *context->opcode;
  *context->interrupt_master_enable = true;
  PrintInstruction("EI");
  return instruction_ptr;
}

int RLCA(handlers::ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  Opcode opcode = *context->opcode;
  context->cpu->flag_struct.rF.C = NthBit(context->cpu->flag_struct.rA, 7);
  context->cpu->flag_struct.rA = context->cpu->flag_struct.rA << 1;
  context->cpu->flag_struct.rF.H = 0;
  SetNFlag(false, context->cpu);
  SetZFlag(context->cpu->flag_struct.rA, context->cpu);
  PrintInstruction("RLCA");
  return instruction_ptr;
}

int RLA(handlers::ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  Opcode opcode = *context->opcode;
  unsigned char carry = context->cpu->flag_struct.rF.C;
  context->cpu->flag_struct.rF.C = NthBit(context->cpu->flag_struct.rA, 7);
  context->cpu->flag_struct.rA = context->cpu->flag_struct.rA << 1;
  context->cpu->flag_struct.rA |= carry;
  context->cpu->flag_struct.rF.H = 0;
  SetNFlag(false, context->cpu);
  SetZFlag(context->cpu->flag_struct.rA, context->cpu);
  PrintInstruction("RLA");
  return instruction_ptr;
}

int RRCA(handlers::ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  Opcode opcode = *context->opcode;
  context->cpu->flag_struct.rF.C = NthBit(context->cpu->flag_struct.rA, 0);
  context->cpu->flag_struct.rA = context->cpu->flag_struct.rA >> 1;
  context->cpu->flag_struct.rF.H = 0;
  SetNFlag(false, context->cpu);
  SetZFlag(context->cpu->flag_struct.rA, context->cpu);
  PrintInstruction("RRCA");
  return instruction_ptr;
}

int RRA(handlers::ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  Opcode opcode = *context->opcode;
  unsigned char carry = context->cpu->flag_struct.rF.C;
  context->cpu->flag_struct.rF.C = NthBit(context->cpu->flag_struct.rA, 0);
  context->cpu->flag_struct.rA = context->cpu->flag_struct.rA >> 1;
  context->cpu->flag_struct.rA |= (carry << 7);
  context->cpu->flag_struct.rF.H = 0;
  SetNFlag(false, context->cpu);
  SetZFlag(context->cpu->flag_struct.rA, context->cpu);
  PrintInstruction("RRA");
  return instruction_ptr;
}

int RLC(handlers::ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  Opcode* opcode = context->opcode;
  unsigned char* reg = (unsigned char*) opcode->reg1;
  context->cpu->flag_struct.rF.C = NthBit(*reg, 7);
  *reg = *reg << 1;
  context->cpu->flag_struct.rF.H = 0;
  SetNFlag(false, context->cpu);
  SetZFlag(*reg, context->cpu);
  PrintInstruction("RLC", RegisterName8(opcode->reg1, context->cpu));
  return instruction_ptr;
}

int RLCAddress(handlers::ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  Opcode* opcode = context->opcode;
  unsigned char value = context->memory_mapper->Read(*opcode->reg1);
  context->cpu->flag_struct.rF.C = NthBit(value, 7);
  value = value << 1;
  context->memory_mapper->Write(*opcode->reg1, value);
  context->cpu->flag_struct.rF.H = 0;
  SetNFlag(false, context->cpu);
  SetZFlag(value, context->cpu);
  PrintInstruction("RLC", "(" + RegisterName16(opcode->reg1, context->cpu) + ")");
  return instruction_ptr;
}

int RL(handlers::ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  Opcode* opcode = context->opcode;
  unsigned char* reg = (unsigned char*) opcode->reg1;
  unsigned char carry = context->cpu->flag_struct.rF.C;
  context->cpu->flag_struct.rF.C = NthBit(*reg, 7);
  *reg = *reg << 1;
  *reg |= carry;
  context->cpu->flag_struct.rF.H = 0;
  SetNFlag(false, context->cpu);
  SetZFlag(*reg, context->cpu);
  PrintInstruction("RL", RegisterName8(opcode->reg1, context->cpu));
  return instruction_ptr;
}

int RLAddress(handlers::ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  Opcode* opcode = context->opcode;
  unsigned char value = context->memory_mapper->Read(*opcode->reg1);
  unsigned char carry = context->cpu->flag_struct.rF.C;
  context->cpu->flag_struct.rF.C = NthBit(value, 7);
  value = value << 1;
  value |= carry;
  context->memory_mapper->Write(*opcode->reg1, value);
  context->cpu->flag_struct.rF.H = 0;
  SetNFlag(false, context->cpu);
  SetZFlag(value, context->cpu);
  PrintInstruction("RL", "(" + RegisterName16(opcode->reg1, context->cpu) + ")");
  return instruction_ptr;
}

int RRC(handlers::ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  Opcode* opcode = context->opcode;
  unsigned char* reg = (unsigned char*) opcode->reg1;
  context->cpu->flag_struct.rF.C = NthBit(*reg, 0);
  *reg = *reg >> 1;
  context->cpu->flag_struct.rF.H = 0;
  SetNFlag(false, context->cpu);
  SetZFlag(*reg, context->cpu);
  PrintInstruction("RRC", RegisterName8(opcode->reg1, context->cpu));
  return instruction_ptr;
}

int RRCAddress(handlers::ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  Opcode* opcode = context->opcode;
  unsigned char value = context->memory_mapper->Read(*opcode->reg1);
  context->cpu->flag_struct.rF.C = NthBit(value, 0);
  value = value >> 1;
  context->memory_mapper->Write(*opcode->reg1, value);
  context->cpu->flag_struct.rF.H = 0;
  SetNFlag(false, context->cpu);
  SetZFlag(value, context->cpu);
  PrintInstruction("RRC", "(" + RegisterName16(opcode->reg1, context->cpu) + ")");
  return instruction_ptr;
}

int RR(handlers::ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  Opcode* opcode = context->opcode;
  unsigned char* reg = (unsigned char*) opcode->reg1;
  unsigned char carry = context->cpu->flag_struct.rF.C;
  context->cpu->flag_struct.rF.C = NthBit(*reg, 0);
  *reg = *reg >> 1;
  *reg |= (carry << 7);
  context->cpu->flag_struct.rF.H = 0;
  SetNFlag(false, context->cpu);
  SetZFlag(*reg, context->cpu);
  PrintInstruction("RR", RegisterName8(opcode->reg1, context->cpu));
  return instruction_ptr;
}

int RRAddress(handlers::ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  Opcode* opcode = context->opcode;
  unsigned char value = context->memory_mapper->Read(*opcode->reg1);
  unsigned char carry = context->cpu->flag_struct.rF.C;
  context->cpu->flag_struct.rF.C = NthBit(value, 0);
  value = value >> 1;
  value |= (carry << 7);
  context->memory_mapper->Write(*opcode->reg1, value);
  context->cpu->flag_struct.rF.H = 0;
  SetNFlag(false, context->cpu);
  SetZFlag(value, context->cpu);
  PrintInstruction("RR", "(" + RegisterName16(opcode->reg1, context->cpu) + ")");
  return instruction_ptr;
}

int SLA(handlers::ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  Opcode* opcode = context->opcode;
  unsigned char* reg = (unsigned char*) opcode->reg1;
  context->cpu->flag_struct.rF.C = NthBit(*reg, 7);
  *reg = *reg << 1;
  context->cpu->flag_struct.rF.H = 0;
  SetNFlag(false, context->cpu);
  SetZFlag(*reg, context->cpu);
  PrintInstruction("SLA", RegisterName8(opcode->reg1, context->cpu));
  return instruction_ptr;
}

int SLAAddress(handlers::ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  Opcode* opcode = context->opcode;
  unsigned char value = context->memory_mapper->Read(*opcode->reg1);
  context->cpu->flag_struct.rF.C = NthBit(value, 7);
  value = value << 1;
  context->memory_mapper->Write(*opcode->reg1, value);
  context->cpu->flag_struct.rF.H = 0;
  SetNFlag(false, context->cpu);
  SetZFlag(value, context->cpu);
  PrintInstruction("SLA", "(" + RegisterName16(opcode->reg1, context->cpu) + ")");
  return instruction_ptr;
}

int SRA(handlers::ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  Opcode* opcode = context->opcode;
  unsigned char* reg = (unsigned char*) opcode->reg1;
  context->cpu->flag_struct.rF.C = NthBit(*reg, 0);
  unsigned char msb = NthBit(*reg, 7) << 7;
  *reg = *reg >> 1;
  *reg |= msb;
  context->cpu->flag_struct.rF.H = 0;
  SetNFlag(false, context->cpu);
  SetZFlag(*reg, context->cpu);
  PrintInstruction("SRA", RegisterName8(opcode->reg1, context->cpu));
  return instruction_ptr;
}

int SRAAddress(handlers::ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  Opcode* opcode = context->opcode;
  unsigned char value = context->memory_mapper->Read(*opcode->reg1);
  context->cpu->flag_struct.rF.C = NthBit(value, 0);
  unsigned char msb = NthBit(value, 7) << 7;
  value = value >> 1;
  value |= msb;
  context->memory_mapper->Write(*opcode->reg1, value);
  context->cpu->flag_struct.rF.H = 0;
  SetNFlag(false, context->cpu);
  SetZFlag(value, context->cpu);
  PrintInstruction("SRA", "(" + RegisterName16(opcode->reg1, context->cpu) + ")");
  return instruction_ptr;
}

int SRL(handlers::ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  Opcode* opcode = context->opcode;
  unsigned char* reg = (unsigned char*) opcode->reg1;
  context->cpu->flag_struct.rF.C = NthBit(*reg, 0);
  *reg = *reg >> 1;
  context->cpu->flag_struct.rF.H = 0;
  SetNFlag(false, context->cpu);
  SetZFlag(*reg, context->cpu);
  PrintInstruction("SRL", RegisterName8(opcode->reg1, context->cpu));
  return instruction_ptr;
}

int SRLAddress(handlers::ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  Opcode* opcode = context->opcode;
  unsigned char value = context->memory_mapper->Read(*opcode->reg1);
  context->cpu->flag_struct.rF.C = NthBit(value, 0);
  value = value >> 1;
  context->memory_mapper->Write(*opcode->reg1, value);
  context->cpu->flag_struct.rF.H = 0;
  SetNFlag(false, context->cpu);
  SetZFlag(value, context->cpu);
  PrintInstruction("SRL", "(" + RegisterName16(opcode->reg1, context->cpu) + ")");
  return instruction_ptr;
}

int Bit(handlers::ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  Opcode* opcode = context->opcode;
  unsigned char* reg = (unsigned char*) opcode->reg1;
  unsigned char bit = NthBit(*reg, context->magic);
  context->cpu->flag_struct.rF.H = 1;
  SetZFlag(bit, context->cpu);
  SetNFlag(false, context->cpu);
  
  PrintInstruction("BIT", Hex(context->magic), RegisterName8(opcode->reg1, context->cpu));
  return instruction_ptr;
}

int BitAddress(handlers::ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  unsigned char val = context->memory_mapper->Read(context->cpu->rHL);
  unsigned char bit = NthBit(val, context->magic);
  context->cpu->flag_struct.rF.H = 1;
  SetZFlag(bit, context->cpu);
  SetNFlag(false, context->cpu);
  
  PrintInstruction("BIT", Hex(context->magic), "(HL)");
  return instruction_ptr;
}
  
int Set(handlers::ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  Opcode* opcode = context->opcode;
  unsigned char* reg = (unsigned char*) opcode->reg1;
  unsigned char bit = context->magic;
  *reg |= (0x1 << bit);
  
  PrintInstruction("SET", Hex(context->magic), RegisterName8(opcode->reg1, context->cpu));
  return instruction_ptr;
}

int Res(handlers::ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  Opcode* opcode = context->opcode;
  unsigned char* reg = (unsigned char*) opcode->reg1;
  unsigned char bit = context->magic;
  *reg &= ~(0x1 << bit);
  
  PrintInstruction("RES", Hex(context->magic), RegisterName8(opcode->reg1, context->cpu));
  return instruction_ptr;
}


int Jump(handlers::ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  Opcode opcode = *context->opcode;
  instruction_ptr = GetAddress16(context->memory_mapper, instruction_ptr);
  
  PrintInstruction("JP", Hex(instruction_ptr));
  return instruction_ptr;
}

int JumpConditional(handlers::ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  Opcode opcode = *context->opcode;
  switch (opcode.opcode_name) {
    case 0xC2:
      PrintInstruction("JP", "NZ", Hex(GetAddress16(context->memory_mapper, instruction_ptr)));
      if (!context->cpu->flag_struct.rF.Z) {
        return Jump(context);
      }
      return instruction_ptr;
    case 0xCA:
      PrintInstruction("JP", "Z", Hex(GetAddress16(context->memory_mapper, instruction_ptr)));
      if (context->cpu->flag_struct.rF.Z) {
        return Jump(context);
      }
      return instruction_ptr;
    case 0xD2:
      PrintInstruction("JP", "NC", Hex(GetAddress16(context->memory_mapper, instruction_ptr)));
      if (!context->cpu->flag_struct.rF.C) {
        return Jump(context);
      }
      return instruction_ptr;
    case 0xDA:
      PrintInstruction("JP", "C", Hex(GetAddress16(context->memory_mapper, instruction_ptr)));
      if (context->cpu->flag_struct.rF.C) {
        return Jump(context);
      }
      return instruction_ptr;
  }
  return instruction_ptr;
}

int JumpHL(handlers::ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  Opcode opcode = *context->opcode;
  instruction_ptr = context->cpu->rHL;
  
  PrintInstruction("JP", "(HL)");
  return instruction_ptr;
}

int JumpRelative(handlers::ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  Opcode opcode = *context->opcode;
  instruction_ptr += 1 + static_cast<char>(GetParameterValue(context->memory_mapper, instruction_ptr));
  LOG(INFO) << "Jumping to " << std::hex << instruction_ptr;
  
  PrintInstruction("JR", Hex(GetParameterValue(context->memory_mapper, instruction_ptr)));
  return instruction_ptr;
}

int JumpRelativeConditional(handlers::ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  Opcode opcode = *context->opcode;
  switch (opcode.opcode_name) {
    case 0x20:
      PrintInstruction("JR", "NZ", Hex(GetParameterValue(context->memory_mapper, instruction_ptr)));
      if (!context->cpu->flag_struct.rF.Z) {
        return JumpRelative(context);
      }
      return instruction_ptr + 1; // Have to account for the 8-bit parameter 
                                  // whether we use it or not.
    case 0x28:
      PrintInstruction("JR", "Z", Hex(GetParameterValue(context->memory_mapper, instruction_ptr)));
      if (context->cpu->flag_struct.rF.Z) {
        return JumpRelative(context);
      }
      return instruction_ptr + 1;
    case 0x30:
      PrintInstruction("JR", "NC", Hex(GetParameterValue(context->memory_mapper, instruction_ptr)));
      if (!context->cpu->flag_struct.rF.C) {
        return JumpRelative(context);
      }
      return instruction_ptr + 1;
    case 0x38:
      PrintInstruction("JR", "C", Hex(GetParameterValue(context->memory_mapper, instruction_ptr)));
      if (context->cpu->flag_struct.rF.C) {
        return JumpRelative(context);
      }
  }
  LOG(INFO) << "Not jumping";
  return instruction_ptr + 1;
}

// TODO(Brendan, Diego, Aaron, Dave): We should make sure we are doing endian
// specific work in functions that we can either swap out at compile time or at
// runtime to preserve correct endianness.
unsigned char GetLSB(unsigned short value) {
  LOG(INFO) << "Pushing, lsb is " << std::hex << 0x0000 + static_cast<unsigned char>(value);
  return static_cast<unsigned char>(value);
}

unsigned char GetMSB(unsigned short value) {
  LOG(INFO) << "Pushing, msb is " << std::hex << 0x0000 + static_cast<unsigned char>(value >> 8);
  return static_cast<unsigned char>(value >> 8);
}

void PushRegister(MemoryMapper* memory_mapper, GB_CPU* cpu, unsigned short* reg) {
  unsigned short* rSP = &cpu->rSP;
  // memory_mapper->Write(0xfffe, GetLSB(*reg));
  --*rSP;
  memory_mapper->Write(*rSP, GetLSB(*reg));
  --*rSP;
  memory_mapper->Write(*rSP, GetMSB(*reg));
}

void PopRegister(MemoryMapper* memory_mapper, GB_CPU* cpu, unsigned short* reg) {
  unsigned short* rSP = &cpu->rSP;
  unsigned short msb = memory_mapper->Read(*rSP);
  ++*rSP;
  unsigned short lsb = memory_mapper->Read(*rSP);
  ++*rSP;
  *reg = (msb << 8) | lsb;
}

int Call(handlers::ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  Opcode opcode = *context->opcode;
  GB_CPU* cpu = context->cpu;
  unsigned short* rPC = &cpu->rPC;

  unsigned short address = GetParameterValue16(context->memory_mapper, instruction_ptr);
  *rPC += 2; // Must take the parameter into account.
  PushRegister(context->memory_mapper, cpu, rPC);

  LOG(INFO) << "Calling address: " << std::hex << address;
  instruction_ptr = address;
  
  PrintInstruction("CALL", Hex(address));
  return instruction_ptr;
}

int CallConditional(handlers::ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  Opcode opcode = *context->opcode;
  switch (opcode.opcode_name) {
    case 0xC4:
      PrintInstruction("CALL", "NZ", Hex(GetParameterValue(context->memory_mapper, instruction_ptr)));
      if (!context->cpu->flag_struct.rF.Z) {
        return Call(context);
      }
      return instruction_ptr;
    case 0xCC:
      PrintInstruction("CALL", "Z", Hex(GetParameterValue(context->memory_mapper, instruction_ptr)));
      if (context->cpu->flag_struct.rF.Z) {
        return Call(context);
      }
      return instruction_ptr;
    case 0xD4:
      PrintInstruction("CALL", "NC", Hex(GetParameterValue(context->memory_mapper, instruction_ptr)));
      if (!context->cpu->flag_struct.rF.C) {
        return Call(context);
      }
      return instruction_ptr;
    case 0xDC:
      PrintInstruction("CALL", "C", Hex(GetParameterValue(context->memory_mapper, instruction_ptr)));
      if (context->cpu->flag_struct.rF.C) {
        return Call(context);
      }
      return instruction_ptr;
  }
  return instruction_ptr;
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

  GB_CPU* cpu = context->cpu;
  PushRegister(context->memory_mapper, cpu, &cpu->rPC);

  LOG(INFO) << "Restarting at address: " << std::hex << instruction_ptr;
  
  PrintInstruction("RST", Hex(opcode.opcode_name));
  return instruction_ptr;
}

int Return(handlers::ExecutorContext* context) {
  LOG(INFO) << "Returning";
  Opcode opcode = *context->opcode;
  PopRegister(context->memory_mapper, context->cpu, &context->cpu->rPC);
  PrintInstruction("RET");
  return context->cpu->rPC;
}

int ReturnConditional(handlers::ExecutorContext* context) {
  LOG(INFO) << "Conditional return";
  int instruction_ptr = *context->instruction_ptr;
  Opcode opcode = *context->opcode;
  switch (opcode.opcode_name) {
    case 0xC0:
      PrintInstruction("RET", "NZ");
      if (!context->cpu->flag_struct.rF.Z) {
        return Return(context);
      }
      return instruction_ptr;
    case 0xC8:
      PrintInstruction("RET", "Z");
      if (context->cpu->flag_struct.rF.Z) {
        return Return(context);
      }
      return instruction_ptr;
    case 0xD0:
      PrintInstruction("RET", "NC");
      if (!context->cpu->flag_struct.rF.C) {
        return Return(context);
      }
      return instruction_ptr;
    case 0xD8:
      PrintInstruction("RET", "C");
      if (context->cpu->flag_struct.rF.C) {
        return Return(context);
      }
  }
  return instruction_ptr;
}

int ReturnInterrupt(handlers::ExecutorContext* context) {
  LOG(INFO) << "Returning from interrupt.";
  ExecutorContext new_context(context);
  EI(context);
  
  PrintInstruction("RETI");
  return Return(context);
}

int LoadN(handlers::ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  Opcode* opcode = context->opcode;
  unsigned char value = GetParameterValue(context->memory_mapper, instruction_ptr);
  *((unsigned char*) opcode->reg1) = value;
  
  PrintInstruction("LD", RegisterName8(opcode->reg1, context->cpu), Hex(value));
  return instruction_ptr + 1;
}

int LoadRR8Bit(handlers::ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  Opcode opcode = *context->opcode;
  unsigned char* reg1 = (unsigned char*)opcode.reg1;
  unsigned char* reg2 = (unsigned char*)opcode.reg2;
  *((unsigned char*) reg1) = *((unsigned char*) reg2);
  
  PrintInstruction("LD", RegisterName8(opcode.reg1, context->cpu), RegisterName8(opcode.reg2, context->cpu));
  return instruction_ptr;
}

int LoadRR8BitAddress(handlers::ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  Opcode opcode = *context->opcode;
  unsigned char* reg1 = (unsigned char*) opcode.reg1;
  *reg1 = context->memory_mapper->Read(*opcode.reg2);
  
  PrintInstruction("LD", RegisterName8(opcode.reg1, context->cpu), "(HL)");
  return instruction_ptr;
}

int LoadRR8BitIntoAddress(handlers::ExecutorContext* context) {
  Opcode opcode = *context->opcode;
  context->memory_mapper->Write(*opcode.reg1, *opcode.reg2);
  
  PrintInstruction("LD", "(HL)", RegisterName8(opcode.reg2, context->cpu));
  return *context->instruction_ptr;
}
  
int Load8BitLiteral(handlers::ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  Opcode opcode = *context->opcode;
  unsigned short val = (unsigned short)GetParameterValue(context->memory_mapper, instruction_ptr);
  context->memory_mapper->Write(*opcode.reg1, val);
  
  PrintInstruction("LD", "(HL)", Hex(GetParameterValue(context->memory_mapper, instruction_ptr)));
  return instruction_ptr + 1;

}
  
int LoadRR16Bit(handlers::ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  Opcode opcode = *context->opcode;
  *opcode.reg1 = *opcode.reg2;
  
  PrintInstruction("LD", RegisterName16(opcode.reg1, context->cpu), RegisterName16(opcode.reg2, context->cpu));
  return instruction_ptr;
}

int LoadAN(handlers::ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  Opcode opcode = *context->opcode;
  context->cpu->flag_struct.rA = context->memory_mapper->Read(*opcode.reg1);
  
  PrintInstruction("LD", "(" + RegisterName16(opcode.reg1, context->cpu) + ")", "A");
  return instruction_ptr;
}

int LoadAN16BitLiteral(handlers::ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  Opcode opcode = *context->opcode;
  unsigned short address = GetParameterValue16(context->memory_mapper, instruction_ptr);
  context->cpu->flag_struct.rA = context->memory_mapper->Read(address);
  
  PrintInstruction("LD", "(" + Hex(address) + ")", "A");
  return instruction_ptr + 2;
}
  
int LoadAN8BitLiteral(handlers::ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  Opcode opcode = *context->opcode;
  context->cpu->flag_struct.rA = GetParameterValue(context->memory_mapper, instruction_ptr);
  
  PrintInstruction("LD", "A", Hex(GetParameterValue(context->memory_mapper, instruction_ptr)));
  return instruction_ptr + 1;
}

int LoadNA(handlers::ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  Opcode opcode = *context->opcode;
  *opcode.reg1 = (unsigned char)context->cpu->flag_struct.rA;
  
  PrintInstruction("LD", RegisterName8(opcode.reg1, context->cpu), "A");
  return instruction_ptr;
}

int LoadNAAddress(handlers::ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  Opcode opcode = *context->opcode;
  context->memory_mapper->Write(*opcode.reg1, context->cpu->flag_struct.rA);
  
  PrintInstruction("LD", "(" + RegisterName16(opcode.reg1, context->cpu) + ")", "A");
  return instruction_ptr;
}
  
int LoadNA16BitLiteral(handlers::ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  unsigned short address = GetParameterValue16(context->memory_mapper, instruction_ptr);
  context->memory_mapper->Write(address, context->cpu->flag_struct.rA);
  
  PrintInstruction("LD", Hex(address), "A");
  return instruction_ptr + 2;
}

int LoadAC(handlers::ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  Opcode opcode = *context->opcode;
  context->cpu->flag_struct.rA = context->memory_mapper->Read(0xFF00 + context->cpu->bc_struct.rC);
  
  PrintInstruction("LD", "A", "(C)");
  return instruction_ptr;
}

int LoadCA(handlers::ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  Opcode opcode = *context->opcode;
  context->memory_mapper->Write(0xFF00 + context->cpu->bc_struct.rC, context->cpu->flag_struct.rA);
  
  PrintInstruction("LD", "(C)", "A");
  return instruction_ptr;
}

int LoadDecAHL(handlers::ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  context->cpu->flag_struct.rA = context->memory_mapper->Read(context->cpu->rHL);
  context->cpu->rHL--;
  
  PrintInstruction("LD", "A", "(HL-)");
  return instruction_ptr;
}

int LoadDecHLA(handlers::ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  context->memory_mapper->Write(context->cpu->rHL, context->cpu->flag_struct.rA);
  context->cpu->rHL--;
  
  PrintInstruction("LD", "(HL-)", "A");
  return instruction_ptr;
}

int LoadIncAHL(handlers::ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  context->cpu->flag_struct.rA = context->memory_mapper->Read(context->cpu->rHL);
  context->cpu->rHL++;
  
  PrintInstruction("LD", "A", "(HL+)");
  return instruction_ptr;
}

int LoadIncHLA(handlers::ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  context->memory_mapper->Write(context->cpu->rHL, context->cpu->flag_struct.rA);
  context->cpu->rHL++;
  
  PrintInstruction("LD", "(HL+)", "A");
  return instruction_ptr;
}

int LoadHNA(handlers::ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  Opcode opcode = *context->opcode;
  MemoryMapper* memory_mapper = context->memory_mapper;
  unsigned short address = 0xFF00 + GetParameterValue(memory_mapper, instruction_ptr);
  unsigned char value = context->cpu->flag_struct.rA;
  memory_mapper->Write(address, value);
  LOG(INFO) << std::hex << 0x0000 + value << " was written to " << std::hex << address;
  
  PrintInstruction("LD", "(0xff00 +" + Hex(GetParameterValue(memory_mapper, instruction_ptr)) + ")", "A");
  return instruction_ptr + 1;
}

int LoadHAN(handlers::ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  Opcode opcode = *context->opcode;
  MemoryMapper* memory_mapper = context->memory_mapper;
  context->cpu->flag_struct.rA = memory_mapper->Read(0xFF00 + GetParameterValue(memory_mapper, instruction_ptr));
  
  PrintInstruction("LD", "A", "(0xff00 + " + Hex(GetParameterValue(memory_mapper, instruction_ptr)) + ")");
  return instruction_ptr + 1;
}

int LoadNN(handlers::ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  Opcode opcode = *context->opcode;
  *opcode.reg1 = GetParameterValue16(context->memory_mapper, instruction_ptr);
  
  PrintInstruction("LD", RegisterName16(opcode.reg1, context->cpu), Hex(GetParameterValue16(context->memory_mapper, instruction_ptr)));
  return instruction_ptr + 2;
}

int LoadSPHL(handlers::ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  Opcode opcode = *context->opcode;
  context->cpu->rSP = context->cpu->rHL;
  
  PrintInstruction("LD", "SP", "HL");
  return instruction_ptr;
}

int LoadHLSP(handlers::ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  Opcode opcode = *context->opcode;
  char val = static_cast<char>(GetParameterValue(context->memory_mapper, instruction_ptr));
  if (val < 0) {
    context->cpu->flag_struct.rF.H = DoesHalfBorrow8(context->cpu->rSP, val);
    context->cpu->flag_struct.rF.C = DoesBorrow8(context->cpu->rSP, val);
  } else {
    context->cpu->flag_struct.rF.H = DoesHalfCarry8(context->cpu->rSP, val);
    context->cpu->flag_struct.rF.C = DoesCarry8(context->cpu->rSP, val);
  }
  context->cpu->rHL = context->cpu->rSP + val;
  context->cpu->flag_struct.rF.Z = 0;
  SetNFlag(false, context->cpu);
  
  PrintInstruction("LDHL", "SP", Hex(GetParameterValue(context->memory_mapper, instruction_ptr)));
  return instruction_ptr + 1;
}

int LoadNNSP(handlers::ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  MemoryMapper* memory_mapper = context->memory_mapper;
  unsigned short address = GetParameterValue16(memory_mapper, instruction_ptr);
  unsigned char lsb = (unsigned char)(0x00FF & context->cpu->rSP);
  unsigned char msb = (unsigned char)((0xFF00 & context->cpu->rSP) >> 8);
  memory_mapper->Write(address++, lsb);
  memory_mapper->Write(address, msb);
  PrintInstruction("LD", "(" + Hex(GetParameterValue16(memory_mapper, instruction_ptr)) + ")", "SP");
  return instruction_ptr + 2;
}

int Push(handlers::ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  Opcode opcode = *context->opcode;
  PushRegister(context->memory_mapper, context->cpu, opcode.reg1);
  PrintInstruction("Push", RegisterName16(opcode.reg1, context->cpu));
  return instruction_ptr;
}

int Pop(handlers::ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  Opcode opcode = *context->opcode;
  PopRegister(context->memory_mapper, context->cpu, opcode.reg1);
  PrintInstruction("Pop", RegisterName16(opcode.reg1, context->cpu));
  return instruction_ptr;
}

int HaltAndCatchFire(handlers::ExecutorContext*) {
  PrintInstruction("HCF");
  endwin();
  LOG(FATAL) << "Setting memory on fire.";
}

} // namespace handlers
} // namespace back_end
