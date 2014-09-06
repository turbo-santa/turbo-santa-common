#ifndef TURBO_SANTA_COMMON_BACK_END_OPCODE_HANDLERS_H_
#define TURBO_SANTA_COMMON_BACK_END_OPCODE_HANDLERS_H_

#include "back_end/opcodes.h"
#include "back_end/registers.h"

namespace back_end {
namespace handlers {

using opcodes::Opcode;

// 8 Bit ALU
int Add8Bit(unsigned char* rom, int instruction_ptr, Opcode opcode);
int Add8BitLiteral(unsigned char* rom, int instruction_ptr, Opcode opcode);
int ADC8Bit(unsigned char* rom, int instruction_ptr, Opcode opcode);
int ADC8BitLiteral(unsigned char* rom, int instruction_ptr, Opcode opcode);
int Sub8Bit(unsigned char* rom, int instruction_ptr, Opcode opcode);
int Sub8BitLiteral(unsigned char* rom, int instruction_ptr, Opcode opcode);
int And8Bit(unsigned char* rom, int instruction_ptr, Opcode opcode);
int And8BitLiteral(unsigned char* rom, int instruction_ptr, Opcode opcode);
int Or8Bit(unsigned char* rom, int instruction_ptr, Opcode opcode);
int Or8BitLiteral(unsigned char* rom, int instruction_ptr, Opcode opcode);
int Xor8Bit(unsigned char* rom, int instruction_ptr, Opcode opcode);
int Xor8BitLiteral(unsigned char* rom, int instruction_ptr, Opcode opcode);
int Cp8Bit(unsigned char* rom, int instruction_ptr, Opcode opcode);
int Cp8BitLiteral(unsigned char* rom, int instruction_ptr, Opcode opcode);
int Inc8Bit(unsigned char* rom, int instruction_ptr, Opcode opcode);
int Dec8Bit(unsigned char* rom, int instruction_ptr, Opcode opcode);

// 16 Bit ALU
int Add16Bit(unsigned char* rom, int instruction_ptr, Opcode opcode);
int AddSPLiteral(unsigned char* rom, int instruction_ptr, Opcode opcode);
int Inc16Bit(unsigned char* rom, int instruction_ptr, Opcode opcode);
int Dec16Bit(unsigned char* rom, int instruction_ptr, Opcode opcode);

// Miscelaneous
int Swap(unsigned char* rom, int instruction_ptr, Opcode opcode);
int DAA(unsigned char* rom, int instruction_ptr, Opcode opcode);
int CPL(unsigned char* rom, int instruction_ptr, Opcode opcode);
int CCF(unsigned char* rom, int instruction_ptr, Opcode opcode);
int SCF(unsigned char* rom, int instruction_ptr, Opcode opcode);
int NOP(unsigned char* rom, int instruction_ptr, Opcode opcode);
int Halt(unsigned char* rom, int instruction_ptr, Opcode opcode);
int Stop(unsigned char* rom, int instruction_ptr, Opcode opcode);
int DI(unsigned char* rom, int instruction_ptr, Opcode opcode);
int EI(unsigned char* rom, int instruction_ptr, Opcode opcode);

} // namespace back_end
} // namespace handlers

#endif // TURBO_SANTA_COMMON_BACK_END_OPCODE_HANDLERS_H_
