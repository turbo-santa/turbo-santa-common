#include "backend/opcode_executor/opcode_handlers.h"
#include "backend/opcode_executor/opcode_executor.h"
#include "backend/opcode_executor/opcodes.h"
#include "submodules/glog/src/glog/logging.h"
#include <ncurses.h>

namespace back_end {
namespace opcode_executor {

using decompiler::ArgumentType;
using decompiler::Instruction;
using decompiler::Parameter;
using decompiler::Register;
using opcodes::Opcode;
using registers::GB_CPU;
using memory::MemoryMapper;
using std::string;

uint16_t* GetRegister16Bit(Register reg, GB_CPU* cpu) {
  switch (reg) {
    case Register::A:
    case Register::B:
    case Register::C:
    case Register::D:
    case Register::E:
    case Register::H:
    case Register::L:
      LOG(FATAL) << "16 bit pointer requested for 8 bit register.";
    case Register::AF:
      return &cpu->rAF;
    case Register::BC:
      return &cpu->rBC;
    case Register::DE:
      return &cpu->rDE;
    case Register::HL:
      return &cpu->rHL;
    case Register::SP:
      return &cpu->rSP;
    case Register::PC:
      return &cpu->rPC;
    case Register::ZF:
    case Register::NF:
    case Register::HF:
    case Register::CF:
    case Register::ZN:
    case Register::NN:
    case Register::HN:
    case Register::CN:
      LOG(FATAL) << "Address of bit field requested.";
  }
}

uint16_t* GetRegister16Bit(Parameter param, GB_CPU* cpu) {
  switch (param.type) {
    case ArgumentType::REGISTER:
      return GetRegister16Bit(param.value.reg, cpu);
    case ArgumentType::VALUE:
    case ArgumentType::EMPTY:
      LOG(FATAL) << "Attempted to dereference non-register value as register.";
  }
}

uint8_t* GetRegister8Bit(Register reg, GB_CPU* cpu) {
  switch (reg) {
    case Register::A:
      return &cpu->flag_struct.rA;
    case Register::B:
      return &cpu->bc_struct.rB;
    case Register::C:
      return &cpu->bc_struct.rC;
    case Register::D:
      return &cpu->de_struct.rD;
    case Register::E:
      return &cpu->de_struct.rE;
    case Register::H:
      return &cpu->hl_struct.rH;
    case Register::L:
      return &cpu->hl_struct.rL;
    case Register::AF:
    case Register::BC:
    case Register::DE:
    case Register::HL:
    case Register::SP:
    case Register::PC:
      LOG(FATAL) << "8 bit pointer requested for 16 bit register.";
    case Register::ZF:
    case Register::NF:
    case Register::HF:
    case Register::CF:
    case Register::ZN:
    case Register::NN:
    case Register::HN:
    case Register::CN:
      LOG(FATAL) << "Address of bit field requested.";
  }
}

uint8_t* GetRegister8Bit(Parameter param, GB_CPU* cpu) {
  switch (param.type) {
    case ArgumentType::REGISTER:
      return GetRegister8Bit(param.value.reg, cpu);
    case ArgumentType::VALUE:
    case ArgumentType::EMPTY:
      LOG(FATAL) << "Attempted to dereference non-register value as register.";
  }
}

uint16_t GetRegisterValue16Bit(Register reg, GB_CPU* cpu) {
  switch (reg) {
    case Register::A:
      return cpu->flag_struct.rA;
    case Register::B:
      return cpu->bc_struct.rB;
    case Register::C:
      return cpu->bc_struct.rC;
    case Register::D:
      return cpu->de_struct.rD;
    case Register::E:
      return cpu->de_struct.rE;
    case Register::H:
      return cpu->hl_struct.rH;
    case Register::L:
      return cpu->hl_struct.rL;
    case Register::AF:
      return cpu->rAF;
    case Register::BC:
      return cpu->rBC;
    case Register::DE:
      return cpu->rDE;
    case Register::HL:
      return cpu->rHL;
    case Register::SP:
      return cpu->rSP;
    case Register::PC:
      return cpu->rPC;
    case Register::ZF:
      return cpu->flag_struct.rF.Z;
    case Register::NF:
      return cpu->flag_struct.rF.N;
    case Register::HF:
      return cpu->flag_struct.rF.H;
    case Register::CF:
      return cpu->flag_struct.rF.C;
    case Register::ZN:
      return cpu->flag_struct.rF.Z;
    case Register::NN:
      return cpu->flag_struct.rF.N;
    case Register::HN:
      return cpu->flag_struct.rF.H;
    case Register::CN:
      return cpu->flag_struct.rF.C;
  }
}

uint8_t GetRegisterValue8Bit(Register reg, GB_CPU* cpu) {
  return static_cast<uint8_t>(GetRegisterValue16Bit(reg, cpu));
}

uint16_t GetParameterValue16Bit(const Parameter& param, GB_CPU* cpu) {
  switch (param.type) {
    case ArgumentType::REGISTER:
      return GetRegisterValue16Bit(param.value.reg, cpu);
    case ArgumentType::VALUE:
      return param.value.val;
    case ArgumentType::EMPTY:
      LOG(FATAL) << "Tried to dereference a non-existent parameter.";
  }
}

uint8_t GetParameterValue8Bit(const Parameter& param, GB_CPU* cpu) {
  return static_cast<uint8_t>(GetParameterValue16Bit(param, cpu));
}

int8_t GetParameterValue8BitSigned(const Parameter& param, GB_CPU* cpu) {
  return static_cast<int8_t>(GetParameterValue8Bit(param, cpu));
}

void SetZFlag(uint8_t register_value, GB_CPU* cpu) {
  cpu->flag_struct.rF.Z = register_value == 0;
}

void SetNFlag(bool performed_subtraction, GB_CPU* cpu) {
  cpu->flag_struct.rF.N = performed_subtraction;
}

uint8_t NthBit(unsigned int byte, int n) {
  return (byte >> n) & 1;
}

string Hex(uint16_t i) {
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

// void PrintInstruction(FrameFactory* frame_factory, string instruction) {
//   LOG(INFO) << "Instruction: " << instruction;
//   frame_factory->SetInstructionName(instruction);
// }
// 
// void PrintInstruction(FrameFactory* frame_factory, string instruction, string arg1) {
//   LOG(INFO) << "Instruction: " << instruction << " " << arg1;
//   frame_factory->SetInstructionName(instruction + " " + arg1);
// }
// 
// void PrintInstruction(FrameFactory* frame_factory, string instruction, string arg1, string arg2) {
//   LOG(INFO) << "Instruction: " << instruction << " " << arg1 << "," << arg2;
//   frame_factory->SetInstructionName(instruction + " " + arg1 + " " + arg2);
// }

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

bool DoesBorrow8(uint8_t left, uint8_t right) {
  return DoesUnderflow(left, right, 7);
}

bool DoesBorrow16(uint16_t left, uint16_t right) {
  return DoesUnderflow(left, right, 15);
}

bool DoesHalfBorrow8(uint8_t left, uint8_t right) {
  return DoesUnderflow(left, right, 3);
}

bool DoesHalfBorrow16(uint16_t left, uint16_t right) {
  return DoesUnderflow(left, right, 11);
}

bool DoesHalfCarry8(uint8_t left, uint8_t right) {
  return DoesOverflow(left, right, 3);
}

bool DoesCarry8(uint8_t left, uint8_t right) {
  return DoesOverflow(left, right, 7);
}

bool DoesHalfCarry16(uint8_t left, uint8_t right) {
  return DoesOverflow(left, right, 11);
}

bool DoesCarry16(unsigned int left, unsigned int right) {
  return DoesOverflow(left, right, 15);
}

void Add8BitImpl(uint8_t value, GB_CPU* cpu) {
  cpu->flag_struct.rF.C = DoesCarry8(cpu->flag_struct.rA, value);
  cpu->flag_struct.rF.H = DoesHalfCarry8(cpu->flag_struct.rA, value);
  cpu->flag_struct.rA += value;
  SetZFlag(cpu->flag_struct.rA, cpu);
  SetNFlag(false, cpu);
}

int Add8Bit(const decompiler::Instruction& instruction, ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  Add8BitImpl(GetParameterValue8Bit(instruction.arg2, context->cpu), context->cpu);
  // PrintInstruction(context->frame_factory, "ADD", "A", RegisterName8(opcode.reg1, context->cpu));
  return instruction_ptr;
}

int Add8BitAddress(const decompiler::Instruction& instruction, ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  const uint16_t address = GetParameterValue16Bit(instruction.arg2, context->cpu);
  Add8BitImpl(context->memory_mapper->Read(address), context->cpu);
  // PrintInstruction(context->frame_factory, "ADD", "A", "(" + RegisterName16(opcode.reg1, context->cpu) + ")");
  return instruction_ptr;
}

int Add8BitLiteral(const decompiler::Instruction& instruction, ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  Add8BitImpl(GetParameterValue8Bit(instruction.arg2, context->cpu), context->cpu);
  // PrintInstruction(context->frame_factory, "ADD", "A", Hex(GetParameterValue(context->memory_mapper, instruction_ptr)));
  return instruction_ptr;
}

void ADC8BitImpl(uint8_t value, GB_CPU* cpu) {
  char carry = cpu->flag_struct.rF.C;
  cpu->flag_struct.rF.C = DoesCarry8(cpu->flag_struct.rA, value + carry);
  cpu->flag_struct.rF.H = DoesHalfCarry8(cpu->flag_struct.rA, value + carry);
  cpu->flag_struct.rA += value;
  cpu->flag_struct.rA += carry;
  SetZFlag(cpu->flag_struct.rA, cpu);
  SetNFlag(false, cpu);
}

int ADC8Bit(const decompiler::Instruction& instruction, ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  ADC8BitImpl(GetParameterValue8Bit(instruction.arg2, context->cpu), context->cpu);
  // PrintInstruction(context->frame_factory, "ADC", "A", RegisterName8(opcode->reg1, context->cpu));
  return instruction_ptr;
}

int ADC8BitAddress(const decompiler::Instruction& instruction, ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  const uint16_t address = GetParameterValue16Bit(instruction.arg2, context->cpu);
  ADC8BitImpl(address, context->cpu);
  // PrintInstruction(context->frame_factory, "ADC", "A", "(" + RegisterName16(opcode->reg1, context->cpu) + ")");
  return instruction_ptr;
}

int ADC8BitLiteral(const decompiler::Instruction& instruction, ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  ADC8BitImpl(GetParameterValue8Bit(instruction.arg2, context->cpu), context->cpu);
  // PrintInstruction(context->frame_factory, "ADC", "A", Hex(GetParameterValue(context->memory_mapper, instruction_ptr)));
  return instruction_ptr;
}

void Sub8BitImpl(uint8_t value, GB_CPU* cpu) {
  cpu->flag_struct.rF.C = !DoesBorrow8(cpu->flag_struct.rA, value);
  cpu->flag_struct.rF.H = !DoesHalfBorrow8(cpu->flag_struct.rA, value);
  cpu->flag_struct.rA -= value;
  SetZFlag(cpu->flag_struct.rA, cpu);
  SetNFlag(true, cpu);
}

int Sub8Bit(const decompiler::Instruction& instruction, ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  Sub8BitImpl(GetParameterValue8Bit(instruction.arg2, context->cpu), context->cpu);
  // PrintInstruction(context->frame_factory, "SUB", "A", RegisterName8(opcode->reg1, context->cpu));
  return instruction_ptr;
}

int Sub8BitAddress(const decompiler::Instruction& instruction, ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  const uint16_t address = GetParameterValue16Bit(instruction.arg2, context->cpu);
  Sub8BitImpl(context->memory_mapper->Read(address), context->cpu);
  // PrintInstruction(context->frame_factory, "SUB", "A", "(" + RegisterName16(opcode->reg1, context->cpu) + ")");
  return instruction_ptr;
}
  
int Sub8BitLiteral(const decompiler::Instruction& instruction, ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  Sub8BitImpl(GetParameterValue8Bit(instruction.arg2, context->cpu), context->cpu);
  // PrintInstruction(context->frame_factory, "SUB", "A", Hex(GetParameterValue(context->memory_mapper, instruction_ptr)));
  return instruction_ptr;
}

void SBC8BitImpl(uint8_t value, GB_CPU* cpu) {
    char carry = cpu->flag_struct.rF.C;
    cpu->flag_struct.rF.C = !DoesBorrow8(cpu->flag_struct.rA, value + carry);
    cpu->flag_struct.rF.H = !DoesHalfBorrow8(cpu->flag_struct.rA, value + carry);
    cpu->flag_struct.rA -= value;
    cpu->flag_struct.rA -= carry;
    SetZFlag(cpu->flag_struct.rA, cpu);
    SetNFlag(true, cpu);
}

int SBC8Bit(const decompiler::Instruction& instruction, ExecutorContext* context) {
    SBC8BitImpl(GetParameterValue8Bit(instruction.arg2, context->cpu), context->cpu);
    // PrintInstruction(context->frame_factory, "SBC", "A", RegisterName8(context->opcode->reg1, context->cpu));
    return *context->instruction_ptr;
}

int SBC8BitAddress(const decompiler::Instruction& instruction, ExecutorContext* context) {
  const uint16_t address = GetParameterValue16Bit(instruction.arg2, context->cpu);
  SBC8BitImpl(context->memory_mapper->Read(address), context->cpu);
  // PrintInstruction(context->frame_factory, "SBC", "A", "(" + RegisterName16(context->opcode->reg1, context->cpu) + ")");
  return *context->instruction_ptr;
}

int SBC8BitLiteral(const decompiler::Instruction& instruction, ExecutorContext* context) {
    SBC8BitImpl(GetParameterValue8Bit(instruction.arg2, context->cpu), context->cpu);
    // PrintInstruction(context->frame_factory, "SBC", "A", Hex(GetParameterValue(context->memory_mapper, *context->instruction_ptr)));
    return *context->instruction_ptr;
}

int And8Bit(const decompiler::Instruction& instruction, ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  context->cpu->flag_struct.rA &= GetParameterValue8Bit(instruction.arg2, context->cpu);
  SetZFlag(context->cpu->flag_struct.rA, context->cpu);
  SetNFlag(false, context->cpu);
  context->cpu->flag_struct.rF.H = 1;
  context->cpu->flag_struct.rF.C = 0;
  // PrintInstruction(context->frame_factory, "AND", "A", RegisterName8(opcode->reg1, context->cpu));
  return instruction_ptr;
}
  
int And8BitAddress(const decompiler::Instruction& instruction, ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  const uint16_t address = GetParameterValue16Bit(instruction.arg2, context->cpu);
  context->cpu->flag_struct.rA &= context->memory_mapper->Read(address);
  SetZFlag(context->cpu->flag_struct.rA, context->cpu);
  SetNFlag(false, context->cpu);
  context->cpu->flag_struct.rF.H = 1;
  context->cpu->flag_struct.rF.C = 0;
  // PrintInstruction(context->frame_factory, "AND", "A", "(" + RegisterName16(opcode->reg1, context->cpu) + ")");
  return instruction_ptr;
}
  
int And8BitLiteral(const decompiler::Instruction& instruction, ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  context->cpu->flag_struct.rA &= GetParameterValue8Bit(instruction.arg2, context->cpu);
  SetZFlag(context->cpu->flag_struct.rA, context->cpu);
  SetNFlag(false, context->cpu);
  context->cpu->flag_struct.rF.H = 1;
  context->cpu->flag_struct.rF.C = 0;
  // PrintInstruction(context->frame_factory, "AND", "A", Hex(GetParameterValue(context->memory_mapper, instruction_ptr)));
  return instruction_ptr;
}

int Or8Bit(const decompiler::Instruction& instruction, ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  context->cpu->flag_struct.rA |= GetParameterValue8Bit(instruction.arg2, context->cpu);
  SetZFlag(context->cpu->flag_struct.rA, context->cpu);
  SetNFlag(false, context->cpu);
  context->cpu->flag_struct.rF.H = 0;
  context->cpu->flag_struct.rF.C = 0;
  // PrintInstruction(context->frame_factory, "OR", "A", RegisterName8(opcode->reg1, context->cpu));
  return instruction_ptr;
}

int Or8BitAddress(const decompiler::Instruction& instruction, ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  const uint16_t address = GetParameterValue16Bit(instruction.arg2, context->cpu);
  context->cpu->flag_struct.rA |= context->memory_mapper->Read(address);
  SetZFlag(context->cpu->flag_struct.rA, context->cpu);
  SetNFlag(false, context->cpu);
  context->cpu->flag_struct.rF.H = 0;
  context->cpu->flag_struct.rF.C = 0;
  // PrintInstruction(context->frame_factory, "OR", "A", "(" + RegisterName16(opcode->reg1, context->cpu) + ")");
  return instruction_ptr;
}
  
int Or8BitLiteral(const decompiler::Instruction& instruction, ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  context->cpu->flag_struct.rA |= GetParameterValue8Bit(instruction.arg2, context->cpu);
  SetZFlag(context->cpu->flag_struct.rA, context->cpu);
  SetNFlag(false, context->cpu);
  context->cpu->flag_struct.rF.H = 0;
  context->cpu->flag_struct.rF.C = 0;
  // PrintInstruction(context->frame_factory, "OR", "A", Hex(GetParameterValue(context->memory_mapper, instruction_ptr)));
  return instruction_ptr;
}

int Xor8Bit(const decompiler::Instruction& instruction, ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  context->cpu->flag_struct.rA ^= GetParameterValue8Bit(instruction.arg2, context->cpu);
  SetZFlag(context->cpu->flag_struct.rA, context->cpu);
  SetNFlag(false, context->cpu);
  context->cpu->flag_struct.rF.H = 0;
  context->cpu->flag_struct.rF.C = 0;
  // PrintInstruction(context->frame_factory, "XOR", "A", RegisterName8(opcode->reg1, context->cpu));
  return instruction_ptr;
}

int Xor8BitAddress(const decompiler::Instruction& instruction, ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  const uint16_t address = GetParameterValue16Bit(instruction.arg2, context->cpu);
  context->cpu->flag_struct.rA ^= context->memory_mapper->Read(address);
  SetZFlag(context->cpu->flag_struct.rA, context->cpu);
  SetNFlag(false, context->cpu);
  context->cpu->flag_struct.rF.H = 0;
  context->cpu->flag_struct.rF.C = 0;
  // PrintInstruction(context->frame_factory, "XOR", "A", "(" + RegisterName16(opcode->reg1, context->cpu) + ")");
  return instruction_ptr;
}

int Xor8BitLiteral(const decompiler::Instruction& instruction, ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  context->cpu->flag_struct.rA ^= GetParameterValue8Bit(instruction.arg2, context->cpu);
  SetZFlag(context->cpu->flag_struct.rA, context->cpu);
  SetNFlag(false, context->cpu);
  context->cpu->flag_struct.rF.H = 0;
  context->cpu->flag_struct.rF.C = 0;
  // PrintInstruction(context->frame_factory, "XOR", "A", Hex(GetParameterValue(context->memory_mapper, instruction_ptr)));
  return instruction_ptr;
}

int Cp8Bit(const decompiler::Instruction& instruction, ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  const uint8_t arg2 = GetParameterValue8Bit(instruction.arg2, context->cpu);
  uint8_t result = context->cpu->flag_struct.rA - arg2;
  SetZFlag(result, context->cpu);
  SetNFlag(true, context->cpu); // Performed subtraction.
  context->cpu->flag_struct.rF.H = !DoesHalfBorrow8(context->cpu->flag_struct.rA, arg2);
  context->cpu->flag_struct.rF.C = !DoesBorrow8(context->cpu->flag_struct.rA, arg2);
  // PrintInstruction(context->frame_factory, "CP", "A", RegisterName8(opcode->reg1, context->cpu));
  LOG(INFO) << "Z flag = " << 0x0000 + context->cpu->flag_struct.rF.Z;
  LOG(INFO) << "N flag = " << 0x0000 + context->cpu->flag_struct.rF.N;
  LOG(INFO) << "H flag = " << 0x0000 + context->cpu->flag_struct.rF.H;
  LOG(INFO) << "C flag = " << 0x0000 + context->cpu->flag_struct.rF.C;
  return instruction_ptr;
}

int Cp8BitAddress(const decompiler::Instruction& instruction, ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  uint8_t value = context->memory_mapper->Read(GetParameterValue16Bit(instruction.arg2, context->cpu));
  uint8_t result = context->cpu->flag_struct.rA - value;
  SetZFlag(result, context->cpu);
  SetNFlag(true, context->cpu); // Performed subtraction.
  context->cpu->flag_struct.rF.H = !DoesHalfBorrow8(context->cpu->flag_struct.rA, value);
  context->cpu->flag_struct.rF.C = !DoesBorrow8(context->cpu->flag_struct.rA, value);
  // PrintInstruction(context->frame_factory, "CP", "A", "(" + RegisterName16(opcode->reg1, context->cpu) + ")");
  LOG(INFO) << "Z flag = " << 0x0000 + context->cpu->flag_struct.rF.Z;
  LOG(INFO) << "N flag = " << 0x0000 + context->cpu->flag_struct.rF.N;
  LOG(INFO) << "H flag = " << 0x0000 + context->cpu->flag_struct.rF.H;
  LOG(INFO) << "C flag = " << 0x0000 + context->cpu->flag_struct.rF.C;
  return instruction_ptr;
}
  
  
int Cp8BitLiteral(const decompiler::Instruction& instruction, ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  const uint8_t value = GetParameterValue8Bit(instruction.arg2, context->cpu);
  uint8_t result = context->cpu->flag_struct.rA - value;
  SetZFlag(result, context->cpu);
  SetNFlag(true, context->cpu); // Performed subtraction.
  context->cpu->flag_struct.rF.H = !DoesHalfBorrow8(context->cpu->flag_struct.rA, value);
  context->cpu->flag_struct.rF.C = !DoesBorrow8(context->cpu->flag_struct.rA, value);
  // PrintInstruction(context->frame_factory, "CP", "A", Hex(GetParameterValue(context->memory_mapper, instruction_ptr)));
  LOG(INFO) << "Z flag = " << 0x0000 + context->cpu->flag_struct.rF.Z;
  LOG(INFO) << "N flag = " << 0x0000 + context->cpu->flag_struct.rF.N;
  LOG(INFO) << "H flag = " << 0x0000 + context->cpu->flag_struct.rF.H;
  LOG(INFO) << "C flag = " << 0x0000 + context->cpu->flag_struct.rF.C;
  return instruction_ptr;
}

int Inc8Bit(const decompiler::Instruction& instruction, ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  uint8_t* reg = GetRegister8Bit(instruction.arg1, context->cpu);
  bool half_carry = DoesHalfCarry8(*reg, 1);
  ++(*reg);
  
  SetZFlag(*reg, context->cpu);
  SetNFlag(false, context->cpu);
  context->cpu->flag_struct.rF.H = half_carry;
  // PrintInstruction(context->frame_factory, "INC", RegisterName8(opcode.reg1, context->cpu));
  return instruction_ptr;
}

int Inc8BitAddress(const decompiler::Instruction& instruction, ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  const uint16_t address = GetParameterValue16Bit(instruction.arg1, context->cpu);
  uint8_t val = context->memory_mapper->Read(address);
  bool half_carry = DoesHalfCarry8(val, 1);
  context->memory_mapper->Write(address, ++val);

  SetZFlag(val, context->cpu);
  SetNFlag(false, context->cpu);
  context->cpu->flag_struct.rF.H = half_carry;
  // PrintInstruction(context->frame_factory, "INC", "(" + RegisterName16(opcode.reg1, context->cpu) + ")");
  return instruction_ptr;
}
  
int Dec8Bit(const decompiler::Instruction& instruction, ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  uint8_t* reg = GetRegister8Bit(instruction.arg1, context->cpu);
  bool borrowed_h = DoesHalfBorrow8(*reg, 1);
  --(*reg);
  
  SetZFlag(*reg, context->cpu);
  SetNFlag(true, context->cpu);
  context->cpu->flag_struct.rF.H = borrowed_h;
  // PrintInstruction(context->frame_factory, "DEC", RegisterName8(opcode.reg1, context->cpu));
  return instruction_ptr;
}

int Dec8BitAddress(const decompiler::Instruction& instruction, ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  const uint16_t address = GetParameterValue16Bit(instruction.arg1, context->cpu);
  uint8_t val = context->memory_mapper->Read(address);
  bool borrowed_h = DoesHalfBorrow8(val, 1);
  context->memory_mapper->Write(address, --val);
  
  SetZFlag(val, context->cpu);
  SetNFlag(true, context->cpu);
  context->cpu->flag_struct.rF.H = borrowed_h;
  // PrintInstruction(context->frame_factory, "DEC", "(" + RegisterName16(opcode.reg1, context->cpu) + ")");
  return instruction_ptr;
}
  
int Add16Bit(const decompiler::Instruction& instruction, ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  const uint16_t arg2 = GetParameterValue16Bit(instruction.arg2, context->cpu);
  context->cpu->flag_struct.rF.H = DoesHalfCarry16(context->cpu->rHL, arg2);
  context->cpu->flag_struct.rF.C = DoesCarry16(context->cpu->rHL, arg2);
  context->cpu->rHL += arg2;
  SetNFlag(false, context->cpu);
  // PrintInstruction(context->frame_factory, "ADD", "HL", RegisterName16(opcode.reg1, context->cpu));
  return instruction_ptr;
}

int AddSPLiteral(const decompiler::Instruction& instruction, ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  // This value is signed.
  char value = GetParameterValue8Bit(instruction.arg2, context->cpu);
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
  // PrintInstruction(context->frame_factory, "ADD", "SP", Hex(GetParameterValue(context->memory_mapper, instruction_ptr)));
  return instruction_ptr;
}

int Inc16Bit(const decompiler::Instruction& instruction, ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  ++(*GetRegister16Bit(instruction.arg1, context->cpu));
  // No flags are affected by this instruction
  // PrintInstruction(context->frame_factory, "INC", RegisterName16(opcode.reg1, context->cpu));
  return instruction_ptr;
}

int Dec16Bit(const decompiler::Instruction& instruction, ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  --(*GetRegister16Bit(instruction.arg1, context->cpu));
  // No flags are affected by this instruction
  // PrintInstruction(context->frame_factory, "DEC", RegisterName16(opcode.reg1, context->cpu));
  return instruction_ptr;
}

int Swap(const decompiler::Instruction& instruction, ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  uint8_t* reg = GetRegister8Bit(instruction.arg1, context->cpu);
  uint8_t value = *reg;
  *reg = (value << 4) | (value >> 4);
  SetZFlag(*reg, context->cpu);
  SetNFlag(false, context->cpu);
  context->cpu->flag_struct.rF.H = 0;
  context->cpu->flag_struct.rF.C = 0;
  // PrintInstruction(context->frame_factory, "SWAP", RegisterName8(opcode->reg1, context->cpu));
  return instruction_ptr;
}
  
int SwapAddress(const decompiler::Instruction& instruction, ExecutorContext* context) {
  const uint16_t address = GetParameterValue16Bit(instruction.arg1, context->cpu);
  uint8_t val = context->memory_mapper->Read(address);
  uint8_t swapVal = (val << 4) | (val >> 4);
  context->memory_mapper->Write(address, swapVal);
  SetZFlag(swapVal, context->cpu);
  SetNFlag(false, context->cpu);
  context->cpu->flag_struct.rF.H = 0;
  context->cpu->flag_struct.rF.C = 0;
  // PrintInstruction(context->frame_factory, "SWAP", "(" + RegisterName16(context->opcode->reg1, context->cpu) + ")");
  return *context->instruction_ptr;
}

int DAA(const decompiler::Instruction&, ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;

  uint8_t mul = 0x6;
  uint8_t sum = context->cpu->flag_struct.rA;
  uint8_t n = sum / 10;
  while (n > 0) {
    sum += n * mul;
    n /= 10;
    mul *= 0x0F;
  }
  context->cpu->flag_struct.rF.C = context->cpu->flag_struct.rA > 0x64;
  context->cpu->flag_struct.rA = sum;
  context->cpu->flag_struct.rF.H = 0;
  SetZFlag(context->cpu->flag_struct.rA, context->cpu);
  // PrintInstruction(context->frame_factory, "DAA");
  return instruction_ptr;
}

int CPL(const decompiler::Instruction&, ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  context->cpu->flag_struct.rA = ~context->cpu->flag_struct.rA;
  context->cpu->flag_struct.rF.H = 1;
  SetNFlag(true, context->cpu);
  // PrintInstruction(context->frame_factory, "CPL");
  return instruction_ptr;
}

int CCF(const decompiler::Instruction&, ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  context->cpu->flag_struct.rA = ~context->cpu->flag_struct.rA;
  context->cpu->flag_struct.rF.C = !context->cpu->flag_struct.rF.C;
  context->cpu->flag_struct.rF.H = 0;
  SetNFlag(false, context->cpu);
  // PrintInstruction(context->frame_factory, "CCF");
  return instruction_ptr;
}

int SCF(const decompiler::Instruction&, ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  context->cpu->flag_struct.rF.C = 1;
  context->cpu->flag_struct.rF.H = 0;
  SetNFlag(false, context->cpu);
  // PrintInstruction(context->frame_factory, "SCF");
  return instruction_ptr;
}

int NOP(const decompiler::Instruction&, ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  // PrintInstruction(context->frame_factory, "NOP");
  return instruction_ptr;
}

int Halt(const decompiler::Instruction&, ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  LOG(WARNING) << "UNINPLEMENTED OPCODE: Halt";
  // TODO: We should actually halt instead of just nop
  // PrintInstruction(context->frame_factory, "HALT");
  return instruction_ptr;
}

int Stop(const decompiler::Instruction&, ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  LOG(WARNING) << "UNINPLEMENTED OPCODE: Stop";
  // TODO: We should actually stop instead of just nop
  // PrintInstruction(context->frame_factory, "STOP");
  return instruction_ptr;
}

int DI(const decompiler::Instruction&, ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  *context->interrupt_master_enable = false;
  // PrintInstruction(context->frame_factory, "DI");
  return instruction_ptr;
}

int EI(const decompiler::Instruction&, ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  *context->interrupt_master_enable = true;
  // PrintInstruction(context->frame_factory, "EI");
  return instruction_ptr;
}

int RLCA(const decompiler::Instruction&, ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  uint8_t msb = NthBit(context->cpu->flag_struct.rA, 7);
  context->cpu->flag_struct.rF.C = msb;
  context->cpu->flag_struct.rA = (context->cpu->flag_struct.rA << 1) | msb;
  context->cpu->flag_struct.rF.H = 0;
  SetNFlag(false, context->cpu);
  SetZFlag(context->cpu->flag_struct.rA, context->cpu);
  // PrintInstruction(context->frame_factory, "RLCA");
  return instruction_ptr;
}

int RLA(const decompiler::Instruction&, ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  uint8_t carry = context->cpu->flag_struct.rF.C;
  context->cpu->flag_struct.rF.C = NthBit(context->cpu->flag_struct.rA, 7);
  context->cpu->flag_struct.rA = context->cpu->flag_struct.rA << 1;
  context->cpu->flag_struct.rA |= carry;
  context->cpu->flag_struct.rF.H = 0;
  SetNFlag(false, context->cpu);
  SetZFlag(context->cpu->flag_struct.rA, context->cpu);
  // PrintInstruction(context->frame_factory, "RLA");
  return instruction_ptr;
}

int RRCA(const decompiler::Instruction&, ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  uint8_t lsb = NthBit(context->cpu->flag_struct.rA, 0);
  context->cpu->flag_struct.rF.C = lsb;
  context->cpu->flag_struct.rA = (context->cpu->flag_struct.rA >> 1) | (lsb << 7);
  context->cpu->flag_struct.rF.H = 0;
  SetNFlag(false, context->cpu);
  SetZFlag(context->cpu->flag_struct.rA, context->cpu);
  // PrintInstruction(context->frame_factory, "RRCA");
  return instruction_ptr;
}

int RRA(const decompiler::Instruction&, ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  uint8_t carry = context->cpu->flag_struct.rF.C;
  context->cpu->flag_struct.rF.C = NthBit(context->cpu->flag_struct.rA, 0);
  context->cpu->flag_struct.rA = context->cpu->flag_struct.rA >> 1;
  context->cpu->flag_struct.rA |= (carry << 7);
  context->cpu->flag_struct.rF.H = 0;
  SetNFlag(false, context->cpu);
  SetZFlag(context->cpu->flag_struct.rA, context->cpu);
  // PrintInstruction(context->frame_factory, "RRA");
  return instruction_ptr;
}

int RLC(const decompiler::Instruction& instruction, ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  uint8_t* reg = GetRegister8Bit(instruction.arg1, context->cpu);
  uint8_t msb = NthBit(*reg, 7);
  context->cpu->flag_struct.rF.C = msb;
  *reg = (*reg << 1) | msb;
  context->cpu->flag_struct.rF.H = 0;
  SetNFlag(false, context->cpu);
  SetZFlag(*reg, context->cpu);
  // PrintInstruction(context->frame_factory, "RLC", RegisterName8(opcode->reg1, context->cpu));
  return instruction_ptr;
}

int RLCAddress(const decompiler::Instruction& instruction, ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  const uint16_t address = GetParameterValue16Bit(instruction.arg1, context->cpu);
  uint8_t value = context->memory_mapper->Read(address);
  uint8_t msb = NthBit(value, 7);
  context->cpu->flag_struct.rF.C = msb;
  value = (value << 1) | msb;
  context->memory_mapper->Write(address, value);
  context->cpu->flag_struct.rF.H = 0;
  SetNFlag(false, context->cpu);
  SetZFlag(value, context->cpu);
  // PrintInstruction(context->frame_factory, "RLC", "(" + RegisterName16(opcode->reg1, context->cpu) + ")");
  return instruction_ptr;
}

int RL(const decompiler::Instruction& instruction, ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  uint8_t* reg = GetRegister8Bit(instruction.arg1, context->cpu);
  uint8_t carry = context->cpu->flag_struct.rF.C;
  context->cpu->flag_struct.rF.C = NthBit(*reg, 7);
  *reg = *reg << 1;
  *reg |= carry;
  context->cpu->flag_struct.rF.H = 0;
  SetNFlag(false, context->cpu);
  SetZFlag(*reg, context->cpu);
  // PrintInstruction(context->frame_factory, "RL", RegisterName8(opcode->reg1, context->cpu));
  return instruction_ptr;
}

int RLAddress(const decompiler::Instruction& instruction, ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  const uint16_t address = GetParameterValue16Bit(instruction.arg1, context->cpu);
  uint8_t value = context->memory_mapper->Read(address);
  uint8_t carry = context->cpu->flag_struct.rF.C;
  context->cpu->flag_struct.rF.C = NthBit(value, 7);
  value = value << 1;
  value |= carry;
  context->memory_mapper->Write(address, value);
  context->cpu->flag_struct.rF.H = 0;
  SetNFlag(false, context->cpu);
  SetZFlag(value, context->cpu);
  // PrintInstruction(context->frame_factory, "RL", "(" + RegisterName16(opcode->reg1, context->cpu) + ")");
  return instruction_ptr;
}

int RRC(const decompiler::Instruction& instruction, ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  uint8_t* reg = GetRegister8Bit(instruction.arg1, context->cpu);
  uint8_t lsb = NthBit(*reg, 0);
  context->cpu->flag_struct.rF.C = lsb;
  *reg = (*reg >> 1) | (lsb << 7);
  context->cpu->flag_struct.rF.H = 0;
  SetNFlag(false, context->cpu);
  SetZFlag(*reg, context->cpu);
  // PrintInstruction(context->frame_factory, "RRC", RegisterName8(opcode->reg1, context->cpu));
  return instruction_ptr;
}

int RRCAddress(const decompiler::Instruction& instruction, ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  const uint16_t address = GetParameterValue16Bit(instruction.arg1, context->cpu);
  uint8_t value = context->memory_mapper->Read(address);
  uint8_t lsb = NthBit(value, 0);
  context->cpu->flag_struct.rF.C = lsb;
  value = (value >> 1) | (lsb << 7);
  context->memory_mapper->Write(address, value);
  context->cpu->flag_struct.rF.H = 0;
  SetNFlag(false, context->cpu);
  SetZFlag(value, context->cpu);
  // PrintInstruction(context->frame_factory, "RRC", "(" + RegisterName16(opcode->reg1, context->cpu) + ")");
  return instruction_ptr;
}

int RR(const decompiler::Instruction& instruction, ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  uint8_t* reg = GetRegister8Bit(instruction.arg1, context->cpu);
  uint8_t carry = context->cpu->flag_struct.rF.C;
  context->cpu->flag_struct.rF.C = NthBit(*reg, 0);
  *reg = *reg >> 1;
  *reg |= (carry << 7);
  context->cpu->flag_struct.rF.H = 0;
  SetNFlag(false, context->cpu);
  SetZFlag(*reg, context->cpu);
  // PrintInstruction(context->frame_factory, "RR", RegisterName8(opcode->reg1, context->cpu));
  return instruction_ptr;
}

int RRAddress(const decompiler::Instruction& instruction, ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  const uint16_t address = GetParameterValue16Bit(instruction.arg1, context->cpu);
  uint8_t value = context->memory_mapper->Read(address);
  uint8_t carry = context->cpu->flag_struct.rF.C;
  context->cpu->flag_struct.rF.C = NthBit(value, 0);
  value = value >> 1;
  value |= (carry << 7);
  context->memory_mapper->Write(address, value);
  context->cpu->flag_struct.rF.H = 0;
  SetNFlag(false, context->cpu);
  SetZFlag(value, context->cpu);
  // PrintInstruction(context->frame_factory, "RR", "(" + RegisterName16(opcode->reg1, context->cpu) + ")");
  return instruction_ptr;
}

int SLA(const decompiler::Instruction& instruction, ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  uint8_t* reg = GetRegister8Bit(instruction.arg1, context->cpu);
  context->cpu->flag_struct.rF.C = NthBit(*reg, 7);
  *reg = *reg << 1;
  context->cpu->flag_struct.rF.H = 0;
  SetNFlag(false, context->cpu);
  SetZFlag(*reg, context->cpu);
  // PrintInstruction(context->frame_factory, "SLA", RegisterName8(opcode->reg1, context->cpu));
  return instruction_ptr;
}

int SLAAddress(const decompiler::Instruction& instruction, ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  const uint16_t address = GetParameterValue16Bit(instruction.arg1, context->cpu);
  uint8_t value = context->memory_mapper->Read(address);
  context->cpu->flag_struct.rF.C = NthBit(value, 7);
  value = value << 1;
  context->memory_mapper->Write(address, value);
  context->cpu->flag_struct.rF.H = 0;
  SetNFlag(false, context->cpu);
  SetZFlag(value, context->cpu);
  // PrintInstruction(context->frame_factory, "SLA", "(" + RegisterName16(opcode->reg1, context->cpu) + ")");
  return instruction_ptr;
}

int SRA(const decompiler::Instruction& instruction, ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  uint8_t* reg = GetRegister8Bit(instruction.arg1, context->cpu);
  context->cpu->flag_struct.rF.C = NthBit(*reg, 0);
  uint8_t msb = NthBit(*reg, 7) << 7;
  *reg = *reg >> 1;
  *reg |= msb;
  context->cpu->flag_struct.rF.H = 0;
  SetNFlag(false, context->cpu);
  SetZFlag(*reg, context->cpu);
  // PrintInstruction(context->frame_factory, "SRA", RegisterName8(opcode->reg1, context->cpu));
  return instruction_ptr;
}

int SRAAddress(const decompiler::Instruction& instruction, ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  const uint16_t address = GetParameterValue16Bit(instruction.arg1, context->cpu);
  uint8_t value = context->memory_mapper->Read(address);
  context->cpu->flag_struct.rF.C = NthBit(value, 0);
  uint8_t msb = NthBit(value, 7) << 7;
  value = value >> 1;
  value |= msb;
  context->memory_mapper->Write(address, value);
  context->cpu->flag_struct.rF.H = 0;
  SetNFlag(false, context->cpu);
  SetZFlag(value, context->cpu);
  // PrintInstruction(context->frame_factory, "SRA", "(" + RegisterName16(opcode->reg1, context->cpu) + ")");
  return instruction_ptr;
}

int SRL(const decompiler::Instruction& instruction, ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  uint8_t* reg = GetRegister8Bit(instruction.arg1, context->cpu);
  context->cpu->flag_struct.rF.C = NthBit(*reg, 0);
  *reg = *reg >> 1;
  context->cpu->flag_struct.rF.H = 0;
  SetNFlag(false, context->cpu);
  SetZFlag(*reg, context->cpu);
  // PrintInstruction(context->frame_factory, "SRL", RegisterName8(opcode->reg1, context->cpu));
  return instruction_ptr;
}

int SRLAddress(const decompiler::Instruction& instruction, ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  const uint16_t address = GetParameterValue16Bit(instruction.arg1, context->cpu);
  uint8_t value = context->memory_mapper->Read(address);
  context->cpu->flag_struct.rF.C = NthBit(value, 0);
  value = value >> 1;
  context->memory_mapper->Write(address, value);
  context->cpu->flag_struct.rF.H = 0;
  SetNFlag(false, context->cpu);
  SetZFlag(value, context->cpu);
  // PrintInstruction(context->frame_factory, "SRL", "(" + RegisterName16(opcode->reg1, context->cpu) + ")");
  return instruction_ptr;
}

int Bit(const decompiler::Instruction& instruction, ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  uint8_t* reg = GetRegister8Bit(instruction.arg2, context->cpu);
  uint8_t bit = NthBit(*reg, GetParameterValue8Bit(instruction.arg1, context->cpu));
  context->cpu->flag_struct.rF.H = 1;
  SetZFlag(bit, context->cpu);
  SetNFlag(false, context->cpu);
  
  // PrintInstruction(context->frame_factory, "BIT", Hex(context->magic), RegisterName8(opcode->reg1, context->cpu));
  return instruction_ptr;
}

int BitAddress(const decompiler::Instruction& instruction, ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  uint8_t val = context->memory_mapper->Read(context->cpu->rHL);
  uint8_t bit = NthBit(val, GetParameterValue8Bit(instruction.arg1, context->cpu));
  context->cpu->flag_struct.rF.H = 1;
  SetZFlag(bit, context->cpu);
  SetNFlag(false, context->cpu);
  
  // PrintInstruction(context->frame_factory, "BIT", Hex(context->magic), "(HL)");
  return instruction_ptr;
}
  
int Set(const decompiler::Instruction& instruction, ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  uint8_t* reg = GetRegister8Bit(instruction.arg2, context->cpu);
  uint8_t bit = GetParameterValue8Bit(instruction.arg1, context->cpu);
  *reg |= (0x1 << bit);
  
  // PrintInstruction(context->frame_factory, "SET", Hex(context->magic), RegisterName8(opcode->reg1, context->cpu));
  return instruction_ptr;
}

int Res(const decompiler::Instruction& instruction, ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  uint8_t* reg = GetRegister8Bit(instruction.arg2, context->cpu);
  uint8_t bit = GetParameterValue8Bit(instruction.arg1, context->cpu);
  *reg &= ~(0x1 << bit);
  
  // PrintInstruction(context->frame_factory, "RES", Hex(context->magic), RegisterName8(opcode->reg1, context->cpu));
  return instruction_ptr;
}


int Jump(const decompiler::Instruction& instruction, ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  instruction_ptr = GetParameterValue16Bit(instruction.arg1, context->cpu);
  
  // PrintInstruction(context->frame_factory, "JP", Hex(instruction_ptr));
  return instruction_ptr;
}

int JumpConditional(const decompiler::Instruction& instruction, ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  switch (instruction.instruction) {
    case 0xC2:
      // PrintInstruction(context->frame_factory, "JP", "NZ", Hex(GetAddress16(context->memory_mapper, instruction_ptr)));
      if (!context->cpu->flag_struct.rF.Z) {
        return GetParameterValue16Bit(instruction.arg2, context->cpu);
      }
      return instruction_ptr;
    case 0xCA:
      // PrintInstruction(context->frame_factory, "JP", "Z", Hex(GetAddress16(context->memory_mapper, instruction_ptr)));
      if (context->cpu->flag_struct.rF.Z) {
        return GetParameterValue16Bit(instruction.arg2, context->cpu);
      }
      return instruction_ptr;
    case 0xD2:
      // PrintInstruction(context->frame_factory, "JP", "NC", Hex(GetAddress16(context->memory_mapper, instruction_ptr)));
      if (!context->cpu->flag_struct.rF.C) {
        return GetParameterValue16Bit(instruction.arg2, context->cpu);
      }
      return instruction_ptr;
    case 0xDA:
      // PrintInstruction(context->frame_factory, "JP", "C", Hex(GetAddress16(context->memory_mapper, instruction_ptr)));
      if (context->cpu->flag_struct.rF.C) {
        return GetParameterValue16Bit(instruction.arg2, context->cpu);
      }
      return instruction_ptr;
    default:
      LOG(FATAL) << "Invalid conditional jump.";
  }
}

int JumpHL(const decompiler::Instruction&, ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  instruction_ptr = context->cpu->rHL;
  
  // PrintInstruction(context->frame_factory, "JP", "(HL)");
  return instruction_ptr;
}

int JumpRelative(const decompiler::Instruction& instruction, ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr + GetParameterValue8BitSigned(instruction.arg1, context->cpu);
  // PrintInstruction(context->frame_factory, "JR", Hex(GetParameterValue(context->memory_mapper, instruction_ptr)));
  return instruction_ptr;
}

int JumpRelativeConditional(const decompiler::Instruction& instruction, ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  switch (instruction.instruction) {
    case 0x20:
      // PrintInstruction(context->frame_factory, "JR", "NZ", Hex(GetParameterValue(context->memory_mapper, instruction_ptr)));
      if (!context->cpu->flag_struct.rF.Z) {
        return instruction_ptr + GetParameterValue8BitSigned(instruction.arg2, context->cpu);
      }
      return instruction_ptr;
    case 0x28:
      // PrintInstruction(context->frame_factory, "JR", "Z", Hex(GetParameterValue(context->memory_mapper, instruction_ptr)));
      if (context->cpu->flag_struct.rF.Z) {
        return instruction_ptr + GetParameterValue8BitSigned(instruction.arg2, context->cpu);
      }
      return instruction_ptr;
    case 0x30:
      // PrintInstruction(context->frame_factory, "JR", "NC", Hex(GetParameterValue(context->memory_mapper, instruction_ptr)));
      if (!context->cpu->flag_struct.rF.C) {
        return instruction_ptr + GetParameterValue8BitSigned(instruction.arg2, context->cpu);
      }
      return instruction_ptr;
    case 0x38:
      // PrintInstruction(context->frame_factory, "JR", "C", Hex(GetParameterValue(context->memory_mapper, instruction_ptr)));
      if (context->cpu->flag_struct.rF.C) {
        return instruction_ptr + GetParameterValue8BitSigned(instruction.arg2, context->cpu);
      }
      return instruction_ptr;
    default:
      LOG(FATAL) << "Unkown instruction.";
  }
}

// TODO(Brendan, Diego, Aaron, Dave): We should make sure we are doing endian
// specific work in functions that we can either swap out at compile time or at
// runtime to preserve correct endianness.
uint8_t GetLSB(uint16_t value) {
  LOG(INFO) << "Pushing, lsb is " << std::hex << 0x0000 + static_cast<uint8_t>(value);
  return static_cast<uint8_t>(value);
}

uint8_t GetMSB(uint16_t value) {
  LOG(INFO) << "Pushing, msb is " << std::hex << 0x0000 + static_cast<uint8_t>(value >> 8);
  return static_cast<uint8_t>(value >> 8);
}

void PushRegister(MemoryMapper* memory_mapper, GB_CPU* cpu, uint16_t* reg) {
  uint16_t* rSP = &cpu->rSP;
  // memory_mapper->Write(0xfffe, GetLSB(*reg));
  --*rSP;
  memory_mapper->Write(*rSP, GetLSB(*reg));
  --*rSP;
  memory_mapper->Write(*rSP, GetMSB(*reg));
}

void PopRegister(MemoryMapper* memory_mapper, GB_CPU* cpu, uint16_t* reg) {
  uint16_t* rSP = &cpu->rSP;
  uint16_t msb = memory_mapper->Read(*rSP);
  ++*rSP;
  uint16_t lsb = memory_mapper->Read(*rSP);
  ++*rSP;
  *reg = (msb << 8) | lsb;
}

int Call(const decompiler::Instruction& instruction, ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  GB_CPU* cpu = context->cpu;

  uint16_t address = GetParameterValue16Bit(instruction.arg1, context->cpu);
  // context->call_stack->Push({context->frame_factory->current_timestamp(), *rPC});
  PushRegister(context->memory_mapper, cpu, &cpu->rPC);

  LOG(INFO) << "Calling address: " << std::hex << address;
  instruction_ptr = address;
  
  // PrintInstruction(context->frame_factory, "CALL", Hex(address));
  return instruction_ptr;
}

int CallConditionalImpl(const decompiler::Instruction& instruction, ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  GB_CPU* cpu = context->cpu;

  uint16_t address = GetParameterValue16Bit(instruction.arg2, context->cpu);
  // context->call_stack->Push({context->frame_factory->current_timestamp(), *rPC});
  PushRegister(context->memory_mapper, cpu, &cpu->rPC);

  LOG(INFO) << "Calling address: " << std::hex << address;
  instruction_ptr = address;
  
  // PrintInstruction(context->frame_factory, "CALL", Hex(address));
  return instruction_ptr;
}

int CallConditional(const decompiler::Instruction& instruction, ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  switch (instruction.instruction) {
    case 0xC4:
      // PrintInstruction(context->frame_factory, "CALL", "NZ", Hex(GetParameterValue(context->memory_mapper, instruction_ptr)));
      if (!context->cpu->flag_struct.rF.Z) {
        return CallConditionalImpl(instruction, context);
      }
      return instruction_ptr;
    case 0xCC:
      // PrintInstruction(context->frame_factory, "CALL", "Z", Hex(GetParameterValue(context->memory_mapper, instruction_ptr)));
      if (context->cpu->flag_struct.rF.Z) {
        return CallConditionalImpl(instruction, context);
      }
      return instruction_ptr;
    case 0xD4:
      // PrintInstruction(context->frame_factory, "CALL", "NC", Hex(GetParameterValue(context->memory_mapper, instruction_ptr)));
      if (!context->cpu->flag_struct.rF.C) {
        return CallConditionalImpl(instruction, context);
      }
      return instruction_ptr;
    case 0xDC:
      // PrintInstruction(context->frame_factory, "CALL", "C", Hex(GetParameterValue(context->memory_mapper, instruction_ptr)));
      if (context->cpu->flag_struct.rF.C) {
        return CallConditionalImpl(instruction, context);
      }
      return instruction_ptr;
  }
  return instruction_ptr;
}

int Restart(const decompiler::Instruction& instruction, ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  uint8_t accum = 0;
  switch (instruction.instruction) {
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
  // context->call_stack->Push({context->frame_factory->current_timestamp(), cpu->rPC});
  PushRegister(context->memory_mapper, cpu, &cpu->rPC);

  LOG(INFO) << "Restarting at address: " << std::hex << instruction_ptr;
  
  // PrintInstruction(context->frame_factory, "RST", Hex(opcode.opcode_name));
  return instruction_ptr;
}

int Return(const decompiler::Instruction&, ExecutorContext* context) {
  LOG(INFO) << "Returning";
  PopRegister(context->memory_mapper, context->cpu, &context->cpu->rPC);
  // PrintInstruction(context->frame_factory, "RET");
  // if (!context->call_stack->PeekCheck(context->cpu->rPC)) {
  //   FunctionCall call = context->call_stack->Pop();
  //   string message = "Should have returned to: address = " + Hex(call.address) + " frame = " + std::to_string(call.timestamp);
  //   context->frame_factory->SetEvent(message);
  //   LOG(WARNING) << message;
  // } else {
  //   context->call_stack->Pop();
  // }
  return context->cpu->rPC;
}

int ReturnConditional(const decompiler::Instruction& instruction, ExecutorContext* context) {
  LOG(INFO) << "Conditional return";
  int instruction_ptr = *context->instruction_ptr;
  switch (instruction.instruction) {
    case 0xC0:
      // PrintInstruction(context->frame_factory, "RET", "NZ");
      if (!context->cpu->flag_struct.rF.Z) {
        return Return(instruction, context);
      }
      return instruction_ptr;
    case 0xC8:
      // PrintInstruction(context->frame_factory, "RET", "Z");
      if (context->cpu->flag_struct.rF.Z) {
        return Return(instruction, context);
      }
      return instruction_ptr;
    case 0xD0:
      // PrintInstruction(context->frame_factory, "RET", "NC");
      if (!context->cpu->flag_struct.rF.C) {
        return Return(instruction, context);
      }
      return instruction_ptr;
    case 0xD8:
      // PrintInstruction(context->frame_factory, "RET", "C");
      if (context->cpu->flag_struct.rF.C) {
        return Return(instruction, context);
      }
  }
  return instruction_ptr;
}

int ReturnInterrupt(const decompiler::Instruction& instruction, ExecutorContext* context) {
  LOG(INFO) << "Returning from interrupt.";
  ExecutorContext new_context(context);
  EI(instruction, context);
  
  // PrintInstruction(context->frame_factory, "RETI");
  return Return(instruction, context);
}

int LoadN(const decompiler::Instruction& instruction, ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  *GetRegister8Bit(instruction.arg1, context->cpu) = GetParameterValue8Bit(instruction.arg2, context->cpu);
  
  // PrintInstruction(context->frame_factory, "LD", RegisterName8(opcode->reg1, context->cpu), Hex(value));
  return instruction_ptr;
}

int LoadRR8Bit(const decompiler::Instruction& instruction, ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  *GetRegister8Bit(instruction.arg1, context->cpu) = GetParameterValue8Bit(instruction.arg2, context->cpu);
  
  // PrintInstruction(context->frame_factory, "LD", RegisterName8(opcode.reg1, context->cpu), RegisterName8(opcode.reg2, context->cpu));
  return instruction_ptr;
}

int LoadRR8BitAddress(const decompiler::Instruction& instruction, ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  uint8_t* reg1 = GetRegister8Bit(instruction.arg1, context->cpu);
  const uint16_t address = GetParameterValue16Bit(instruction.arg1, context->cpu);
  *reg1 = context->memory_mapper->Read(address);
  
  // PrintInstruction(context->frame_factory, "LD", RegisterName8(opcode.reg1, context->cpu), "(HL)");
  return instruction_ptr;
}

int LoadRR8BitIntoAddress(const decompiler::Instruction& instruction, ExecutorContext* context) {
  context->memory_mapper->Write(GetParameterValue16Bit(instruction.arg1, context->cpu), GetParameterValue8Bit(instruction.arg1, context->cpu));
  
  // PrintInstruction(context->frame_factory, "LD", "(HL)", RegisterName8(opcode.reg2, context->cpu));
  return *context->instruction_ptr;
}
  
int Load8BitLiteral(const decompiler::Instruction& instruction, ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  uint16_t val = GetParameterValue8Bit(instruction.arg2, context->cpu);
  context->memory_mapper->Write(GetParameterValue16Bit(instruction.arg1, context->cpu), val);
  
  // PrintInstruction(context->frame_factory, "LD", "(HL)", Hex(GetParameterValue(context->memory_mapper, instruction_ptr)));
  return instruction_ptr;

}
  
int LoadRR16Bit(const decompiler::Instruction& instruction, ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  *GetRegister16Bit(instruction.arg1, context->cpu) = GetParameterValue16Bit(instruction.arg2, context->cpu);
  
  // PrintInstruction(context->frame_factory, "LD", RegisterName16(opcode.reg1, context->cpu), RegisterName16(opcode.reg2, context->cpu));
  return instruction_ptr;
}

int LoadAN(const decompiler::Instruction& instruction, ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  const uint16_t address = GetParameterValue16Bit(instruction.arg1, context->cpu);
  context->cpu->flag_struct.rA = context->memory_mapper->Read(address);
  
  // PrintInstruction(context->frame_factory, "LD", "A", "(" + RegisterName16(opcode.reg2, context->cpu) + ")");
  return instruction_ptr;
}

int LoadAN16BitLiteral(const decompiler::Instruction& instruction, ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  uint16_t address = GetParameterValue16Bit(instruction.arg2, context->cpu);
  context->cpu->flag_struct.rA = context->memory_mapper->Read(address);
  
  // PrintInstruction(context->frame_factory, "LD", "A", "(" + Hex(address) + ")");
  return instruction_ptr;
}
  
int LoadAN8BitLiteral(const decompiler::Instruction& instruction, ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  context->cpu->flag_struct.rA = GetParameterValue8Bit(instruction.arg2, context->cpu);
  
  // PrintInstruction(context->frame_factory, "LD", "A", Hex(GetParameterValue(context->memory_mapper, instruction_ptr)));
  return instruction_ptr;
}

int LoadNA(const decompiler::Instruction& instruction, ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  uint8_t* reg1 = GetRegister8Bit(instruction.arg1, context->cpu);
  *reg1 = (uint8_t)context->cpu->flag_struct.rA;
  
  // PrintInstruction(context->frame_factory, "LD", RegisterName8(opcode.reg1, context->cpu), "A");
  return instruction_ptr;
}

int LoadNAAddress(const decompiler::Instruction& instruction, ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  const uint16_t address = GetParameterValue16Bit(instruction.arg1, context->cpu);
  context->memory_mapper->Write(address, context->cpu->flag_struct.rA);
  
  // PrintInstruction(context->frame_factory, "LD", "(" + RegisterName16(opcode.reg1, context->cpu) + ")", "A");
  return instruction_ptr;
}
  
int LoadNA16BitLiteral(const decompiler::Instruction& instruction, ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  uint16_t address = GetParameterValue16Bit(instruction.arg1, context->cpu);
  context->memory_mapper->Write(address, context->cpu->flag_struct.rA);
  
  // PrintInstruction(context->frame_factory, "LD", Hex(address), "A");
  return instruction_ptr;
}

int LoadAC(const decompiler::Instruction&, ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  context->cpu->flag_struct.rA = context->memory_mapper->Read(0xFF00 + context->cpu->bc_struct.rC);
  
  // PrintInstruction(context->frame_factory, "LD", "A", "(C)");
  return instruction_ptr;
}

int LoadCA(const decompiler::Instruction&, ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  context->memory_mapper->Write(0xFF00 + context->cpu->bc_struct.rC, context->cpu->flag_struct.rA);
  
  // PrintInstruction(context->frame_factory, "LD", "(C)", "A");
  return instruction_ptr;
}

int LoadDecAHL(const decompiler::Instruction&, ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  context->cpu->flag_struct.rA = context->memory_mapper->Read(context->cpu->rHL);
  context->cpu->rHL--;
  
  // PrintInstruction(context->frame_factory, "LD", "A", "(HL-)");
  return instruction_ptr;
}

int LoadDecHLA(const decompiler::Instruction&, ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  context->memory_mapper->Write(context->cpu->rHL, context->cpu->flag_struct.rA);
  context->cpu->rHL--;
  
  // PrintInstruction(context->frame_factory, "LD", "(HL-)", "A");
  return instruction_ptr;
}

int LoadIncAHL(const decompiler::Instruction&, ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  context->cpu->flag_struct.rA = context->memory_mapper->Read(context->cpu->rHL);
  context->cpu->rHL++;
  
  // PrintInstruction(context->frame_factory, "LD", "A", "(HL+)");
  return instruction_ptr;
}

int LoadIncHLA(const decompiler::Instruction&, ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  context->memory_mapper->Write(context->cpu->rHL, context->cpu->flag_struct.rA);
  context->cpu->rHL++;
  
  // PrintInstruction(context->frame_factory, "LD", "(HL+)", "A");
  return instruction_ptr;
}

int LoadHNA(const decompiler::Instruction& instruction, ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  MemoryMapper* memory_mapper = context->memory_mapper;
  uint16_t address = 0xFF00 + GetParameterValue8Bit(instruction.arg1, context->cpu);
  uint8_t value = context->cpu->flag_struct.rA;
  memory_mapper->Write(address, value);
  
  // PrintInstruction(context->frame_factory, "LD", "(0xff00 +" + Hex(GetParameterValue(memory_mapper, instruction_ptr)) + ")", "A");
  return instruction_ptr;
}

int LoadHAN(const decompiler::Instruction& instruction, ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  MemoryMapper* memory_mapper = context->memory_mapper;
  context->cpu->flag_struct.rA = memory_mapper->Read(0xFF00 + GetParameterValue8Bit(instruction.arg2, context->cpu));
  
  // PrintInstruction(context->frame_factory, "LD", "A", "(0xff00 + " + Hex(GetParameterValue(memory_mapper, instruction_ptr)) + ")");
  return instruction_ptr;
}

int LoadNN(const decompiler::Instruction& instruction, ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  *GetRegister16Bit(instruction.arg1, context->cpu) = GetParameterValue16Bit(instruction.arg2, context->cpu);
  
  // PrintInstruction(context->frame_factory, "LD", RegisterName16(opcode.reg1, context->cpu), Hex(GetParameterValue16(context->memory_mapper, instruction_ptr)));
  return instruction_ptr;
}

int LoadSPHL(const decompiler::Instruction&, ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  context->cpu->rSP = context->cpu->rHL;
  
  // PrintInstruction(context->frame_factory, "LD", "SP", "HL");
  return instruction_ptr;
}

int LoadHLSP(const decompiler::Instruction& instruction, ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  char val = GetParameterValue8Bit(instruction.arg2, context->cpu);
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
  
  // PrintInstruction(context->frame_factory, "LDHL", "SP", Hex(GetParameterValue(context->memory_mapper, instruction_ptr)));
  return instruction_ptr;
}

int LoadNNSP(const decompiler::Instruction& instruction, ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  MemoryMapper* memory_mapper = context->memory_mapper;
  uint16_t address = GetParameterValue16Bit(instruction.arg1, context->cpu);
  uint8_t lsb = (uint8_t)(0x00FF & context->cpu->rSP);
  uint8_t msb = (uint8_t)((0xFF00 & context->cpu->rSP) >> 8);
  memory_mapper->Write(address++, lsb);
  memory_mapper->Write(address, msb);
  // PrintInstruction(context->frame_factory, "LD", "(" + Hex(GetParameterValue16(memory_mapper, instruction_ptr)) + ")", "SP");
  return instruction_ptr;
}

int Push(const decompiler::Instruction& instruction, ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  PushRegister(context->memory_mapper, context->cpu, GetRegister16Bit(instruction.arg1, context->cpu));
  // PrintInstruction(context->frame_factory, "Push", RegisterName16(opcode.reg1, context->cpu));
  return instruction_ptr;
}

int Pop(const decompiler::Instruction& instruction, ExecutorContext* context) {
  int instruction_ptr = *context->instruction_ptr;
  PopRegister(context->memory_mapper, context->cpu, GetRegister16Bit(instruction.arg1, context->cpu));
  // PrintInstruction(context->frame_factory, "Pop", RegisterName16(opcode.reg1, context->cpu));
  return instruction_ptr;
}

int HaltAndCatchFire(handlers::ExecutorContext*) {
  // PrintInstruction(context->frame_factory, "HCF");
  endwin();
  LOG(FATAL) << "Setting memory on fire.";
}

} // namespace opcode_executor
} // namespace back_end
