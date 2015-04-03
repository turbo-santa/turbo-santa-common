#ifndef TURBO_SANTA_COMMON_BACKEND_INSTRUCTION_FACTORY_H_
#define TURBO_SANTA_COMMON_BACKEND_INSTRUCTION_FACTORY_H_

#include <cstdint>
#include <memory>
#include <string>
#include "backend/decompiler/instruction.h"
#include "backend/decompiler/raw_instruction.h"

namespace back_end {
namespace decompiler {

class ParameterFactory {
 public:
  ParameterFactory(ArgumentType type, bool is_pointer, ValueWidth width)
      : type_(type), is_pointer_(is_pointer), width_(width) {}

  virtual Parameter Build(const RawInstructionBase& instruction) const = 0;

  ArgumentType type() const { return type_; }
  bool is_pointer() const { return is_pointer_; }
  ValueWidth width() const { return width_; }

 private:
  ArgumentType type_;
  bool is_pointer_;
  ValueWidth width_;
};

class RegisterParameterFactory : public ParameterFactory {
 public:
  RegisterParameterFactory(Register reg, bool is_pointer)
      : ParameterFactory(ArgumentType::REGISTER, is_pointer, ValueWidth::BIT_0), reg_(reg) {}

  Parameter Build(const RawInstructionBase&) const {
    return Parameter(ArgumentValue(reg_), type(), is_pointer());
  }

 private:
  Register reg_;
};

class ValueParameterFactory : public ParameterFactory {
 public:
  ValueParameterFactory(ValueWidth width, bool is_pointer)
      : ParameterFactory(ArgumentType::VALUE, is_pointer, width) {}

  Parameter Build(const RawInstructionBase& instruction) const {
    return Parameter(ArgumentValue(instruction.param(width())), type(), is_pointer());
  }
};

class BitParameterFactory : public ParameterFactory {
 public:
  BitParameterFactory()
      : ParameterFactory(ArgumentType::VALUE, false, ValueWidth::BIT_0) {}

  Parameter Build(const RawInstructionBase& instruction) const {
    uint8_t opcode_base = static_cast<uint8_t>(instruction.instr_16bit() >> 8);
    uint8_t bit = (0b00111000 & opcode_base) >> 3;
    return Parameter(ArgumentValue(bit), type(), is_pointer());
  }
};

class ConstantParameterFactory : public ParameterFactory {
 public:
  ConstantParameterFactory(uint16_t value) 
      : ParameterFactory(ArgumentType::VALUE, false, ValueWidth::BIT_0), value_(value) {}

  Parameter Build(const RawInstructionBase&) const {
    return Parameter(ArgumentValue(value_), type(), is_pointer());
  }

 private:
  uint16_t value_;
};

class EmptyParameterFactory : public ParameterFactory {
 public:
  EmptyParameterFactory() : ParameterFactory(ArgumentType::EMPTY, false, ValueWidth::BIT_0) {}

  Parameter Build(const RawInstructionBase&) const {
    return Parameter(ArgumentValue(0), type(), is_pointer());
  }
};

class InstructionFactory {
 public:
  InstructionFactory() {}

  InstructionFactory(Opcode opcode,
                     uint16_t instruction,
                     std::unique_ptr<ParameterFactory> left_param_factory,
                     std::unique_ptr<ParameterFactory> right_param_factory,
                     bool is_jump,
                     ValueWidth instr_width) :
      opcode_(opcode),
      instruction_(instruction),
      left_param_factory_(std::move(left_param_factory)),
      right_param_factory_(std::move(right_param_factory)),
      is_jump_(is_jump),
      instr_width_(instr_width) {}

  InstructionFactory(InstructionFactory&& factory) :
      opcode_(factory.opcode()),
      instruction_(factory.instruction()),
      left_param_factory_(std::move(factory.left_param_factory_)),
      right_param_factory_(std::move(factory.right_param_factory_)),
      is_jump_(factory.is_jump()),
      instr_width_(factory.instr_width()) {}

  void operator=(InstructionFactory&& factory) {
    opcode_ = factory.opcode();
    instruction_ = factory.instruction();
    left_param_factory_ = std::move(factory.left_param_factory_);
    right_param_factory_ = std::move(factory.right_param_factory_);
    is_jump_ = factory.is_jump();
    instr_width_ = factory.instr_width();
  }

  virtual Instruction Build(const RawInstructionBase& raw_instruction) const {
    Parameter left = left_param_factory_->Build(raw_instruction);
    Parameter right = right_param_factory_->Build(raw_instruction);
    return Instruction(opcode_,
                       instruction_,
                       left,
                       right,
                       is_jump_,
                       to_width_bytes(total_width()));
  }

  Opcode opcode() const { return opcode_; }
  uint16_t instruction() const { return instruction_; }
  bool is_jump() const { return is_jump_; }
  ValueWidth instr_width() const { return instr_width_; }
  ValueWidth param_width() const {
    if (!left_param_factory_) {
      LOG(FATAL) << "Left ParameterFactory is null, instruction = 0x" << std::hex << instruction();
    }
    if (!right_param_factory_) {
      LOG(FATAL) << "Right ParameterFactory is null, instruction = 0x" << std::hex << instruction();
    }
    return left_param_factory_->width() + right_param_factory_->width();
  }
  ValueWidth total_width() const { return instr_width_ + param_width(); }

 private:
  Opcode opcode_;
  uint16_t instruction_;
  std::unique_ptr<ParameterFactory> left_param_factory_;
  std::unique_ptr<ParameterFactory> right_param_factory_;
  bool is_jump_;
  ValueWidth instr_width_;
};

InstructionFactory instr(Opcode opcode, 
                  uint16_t instruction, 
                  std::unique_ptr<ParameterFactory> left_param = 
                    std::unique_ptr<ParameterFactory>(new EmptyParameterFactory()),
                  std::unique_ptr<ParameterFactory> right_param = 
                    std::unique_ptr<ParameterFactory>(new EmptyParameterFactory()),
                  bool is_jump = false,
                  ValueWidth width = ValueWidth::BIT_8);

InstructionFactory instr(Opcode opcode, 
                  uint16_t instruction, 
                  std::unique_ptr<ParameterFactory> left_param,
                  std::unique_ptr<ParameterFactory> right_param,
                  ValueWidth width);

InstructionFactory instr(Opcode opcode, 
                  uint16_t instruction, 
                  std::unique_ptr<ParameterFactory> left_param,
                  ValueWidth width);

InstructionFactory instr(Opcode opcode, 
                  uint16_t instruction, 
                  ValueWidth width);

InstructionFactory jump(Opcode opcode, 
                  uint16_t instruction, 
                  std::unique_ptr<ParameterFactory> left_param = 
                    std::unique_ptr<ParameterFactory>(new EmptyParameterFactory()),
                  std::unique_ptr<ParameterFactory> right_param = 
                    std::unique_ptr<ParameterFactory>(new EmptyParameterFactory()),
                  ValueWidth width = ValueWidth::BIT_8);

std::unique_ptr<ParameterFactory> val(Register reg);

std::unique_ptr<ParameterFactory> val(ValueWidth width);

std::unique_ptr<ParameterFactory> ptr(Register reg);

std::unique_ptr<ParameterFactory> ptr(ValueWidth width);

std::unique_ptr<ParameterFactory> bit();

std::unique_ptr<ParameterFactory> con(uint16_t value);

} // namespace decompiler
} // namespace decompiler

#endif // TURBO_SANTA_COMMON_BACKEND_INSTRUCTION_FACTORY_H_
