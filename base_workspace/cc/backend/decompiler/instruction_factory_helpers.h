#ifndef TURBO_SANTA_COMMON_BACKEND_INSTRUCTION_FACTORY_HELPERS_H_
#define TURBO_SANTA_COMMON_BACKEND_INSTRUCTION_FACTORY_HELPERS_H_

#include <cstdint>
#include <memory>
#include "cc/backend/decompiler/instruction.h"
#include "cc/backend/decompiler/instruction_factory.h"
#include "cc/backend/decompiler/raw_instruction.h"


namespace backend {
namespace decompiler {

InstructionFactory instr(Opcode opcode, 
                         uint16_t instruction, 
                         uint16_t clock_cycles,
                         std::unique_ptr<ParameterFactory> left_param = 
                         std::unique_ptr<ParameterFactory>(new EmptyParameterFactory()),
                         std::unique_ptr<ParameterFactory> right_param = 
                         std::unique_ptr<ParameterFactory>(new EmptyParameterFactory()),
                         bool is_jump = false,
                         ValueWidth width = ValueWidth::BIT_8);

InstructionFactory instr(Opcode opcode, 
                         uint16_t instruction,
                         uint16_t clock_cycles,
                         std::unique_ptr<ParameterFactory> left_param,
                         std::unique_ptr<ParameterFactory> right_param,
                         ValueWidth width);

InstructionFactory instr(Opcode opcode, 
                         uint16_t instruction,
                         uint16_t clock_cycles,
                         std::unique_ptr<ParameterFactory> left_param,
                         ValueWidth width);

InstructionFactory instr(Opcode opcode,
                         uint16_t instruction,
                         uint16_t clock_cycles,
                         ValueWidth width);

InstructionFactory jump(Opcode opcode, 
                        uint16_t instruction,
                        uint16_t clock_cycles,
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
} // namespace backend

#endif // TURBO_SANTA_COMMON_BACKEND_INSTRUCTION_FACTORY_HELPERS_H_
