#include "cc/backend/decompiler/instruction_factory_helpers.h"

namespace backend {
namespace decompiler {

InstructionFactory instr(Opcode opcode, 
                         uint16_t instruction,
                         uint16_t clock_cycles,
                         std::unique_ptr<ParameterFactory> left_param,
                         std::unique_ptr<ParameterFactory> right_param,
                         bool is_jump,
                         ValueWidth width) {
  return InstructionFactory(opcode,
                            instruction,
                            clock_cycles,
                            std::move(left_param),
                            std::move(right_param),
                            is_jump,
                            width);
}

InstructionFactory instr(Opcode opcode, 
                         uint16_t instruction,
                         uint16_t clock_cycles,
                         std::unique_ptr<ParameterFactory> left_param,
                         std::unique_ptr<ParameterFactory> right_param,
                         ValueWidth width) {
  return InstructionFactory(opcode,
                            instruction,
                            clock_cycles,
                            std::move(left_param),
                            std::move(right_param),
                            false,
                            width);
}

InstructionFactory instr(Opcode opcode, 
                         uint16_t instruction,
                         uint16_t clock_cycles,
                         std::unique_ptr<ParameterFactory> left_param,
                         ValueWidth width) {
  return InstructionFactory(opcode,
                            instruction,
                            clock_cycles,
                            std::move(left_param),
                            std::unique_ptr<ParameterFactory>(new EmptyParameterFactory()),
                            false,
                            width);
}

InstructionFactory instr(Opcode opcode, 
                         uint16_t instruction,
                         uint16_t clock_cycles,
                         ValueWidth width) {
  return InstructionFactory(opcode,
                            instruction,
                            clock_cycles,
                            std::unique_ptr<ParameterFactory>(new EmptyParameterFactory()),
                            std::unique_ptr<ParameterFactory>(new EmptyParameterFactory()),
                            false,
                            width);
}

InstructionFactory jump(Opcode opcode, 
                        uint16_t instruction,
                        uint16_t clock_cycles,
                        std::unique_ptr<ParameterFactory> left_param, 
                        std::unique_ptr<ParameterFactory> right_param,
                        ValueWidth width) {
  return InstructionFactory(opcode,
                            instruction,
                            clock_cycles,
                            std::move(left_param),
                            std::move(right_param),
                            true,
                            width);
}

std::unique_ptr<ParameterFactory> val(Register reg) {
  return std::unique_ptr<ParameterFactory>(new RegisterParameterFactory(reg, false));
}

std::unique_ptr<ParameterFactory> val(ValueWidth width) {
  return std::unique_ptr<ParameterFactory>(new ValueParameterFactory(width, false));
}

std::unique_ptr<ParameterFactory> ptr(Register reg) {
  return std::unique_ptr<ParameterFactory>(new RegisterParameterFactory(reg, true));
}

std::unique_ptr<ParameterFactory> ptr(ValueWidth width) {
  return std::unique_ptr<ParameterFactory>(new ValueParameterFactory(width, true));
}

std::unique_ptr<ParameterFactory> bit() {
  return std::unique_ptr<ParameterFactory>(new BitParameterFactory());
}

std::unique_ptr<ParameterFactory> con(uint16_t value) {
  return std::unique_ptr<ParameterFactory>(new ConstantParameterFactory(value));
}

} // namespace decompiler
} // namespace backend
