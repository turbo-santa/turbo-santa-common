#ifndef TURBO_SANTA_COMMON_BACKEND_INSTRUCTION_H_
#define TURBO_SANTA_COMMON_BACKEND_INSTRUCTION_H_

#include <cstdint>
#include <string>

namespace back_end {
namespace decompiler {

enum class Opcode {
  ADC,
  ADD,
  AND,
  BIT,
  CALL,
  CCF,
  CP,
  CPL,
  DAA,
  DEC,
  DI,
  EI,
  HALT,
  INC,
  JP,
  JR,
  LD,
  LDD,
  LDHL,
  LDI,
  NOP,
  OR,
  POP,
  PUSH,
  RES,
  RET,
  RETI,
  RL,
  RLA,
  RLC,
  RLCA,
  RR,
  RRA,
  RRC,
  RRCA,
  RST,
  SBC,
  SCF,
  SET,
  SLA,
  SRA,
  SRL,
  STOP,
  SUB,
  SWAP,
  XOR,
};

enum class Register {
  A,
  B,
  C,
  D,
  E,
  H,
  L,
  AF,
  BC,
  DE,
  HL,
  SP,
  PC,
  ZF,
  NF,
  HF,
  CF,
  ZN,
  NN,
  HN,
  CN,
};

union ArgumentValue {
  ArgumentValue() {}
  ArgumentValue(Register reg_) : reg(reg_) {}
  ArgumentValue(uint16_t val_) : val(val_) {}

  Register reg;
  uint16_t val;
};

enum class ArgumentType {
  REGISTER,
  VALUE,
  EMPTY
};

struct Parameter {
  Parameter() : type(ArgumentType::EMPTY) {}

  Parameter(ArgumentValue value_, ArgumentType type_, bool is_pointer_)
      : value(value_), type(type_), is_pointer(is_pointer_) {}

  ArgumentValue value;
  ArgumentType type;
  bool is_pointer;
};

struct Instruction {
  Instruction() {}

  Instruction(Opcode opcode_,
              uint16_t instruction_,
              Parameter arg1_,
              Parameter arg2_,
              bool is_jump_,
              uint16_t instruction_width_bytes_) : 
      opcode(opcode_),
      instruction(instruction_),
      arg1(arg1_),
      arg2(arg2_),
      is_jump(is_jump_),
      instruction_width_bytes(instruction_width_bytes_) {}

  Opcode opcode;
  uint16_t instruction;
  Parameter arg1;
  Parameter arg2;
  bool is_jump;
  uint16_t instruction_width_bytes;
};

} // namespace decompiler
} // namespace decompiler

#endif // TURBO_SANTA_COMMON_BACKEND_INSTRUCTION_H_
