#ifndef TURBO_SANTA_COMMON_BACK_END_OPCODE_HANDLERS_H_
#define TURBO_SANTA_COMMON_BACK_END_OPCODE_HANDLERS_H_

#include "back_end/opcodes.h"
#include "back_end/registers.h"

namespace back_end {
namespace handlers {

using opcodes::Opcode;
using back_end::memory::MemoryMapper;

MemoryMapper mem_map;

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

// Rotates & Shifts
int RLCA(unsigned char* rom, int instruction_ptr, Opcode opcode);
int RLA(unsigned char* rom, int instruction_ptr, Opcode opcode);
int RRCA(unsigned char* rom, int instruction_ptr, Opcode opcode);
int RRA(unsigned char* rom, int instruction_ptr, Opcode opcode);
int RLC(unsigned char* rom, int instruction_ptr, Opcode opcode);
int RL(unsigned char* rom, int instruction_ptr, Opcode opcode);
int RRC(unsigned char* rom, int instruction_ptr, Opcode opcode);
int RR(unsigned char* rom, int instruction_ptr, Opcode opcode);
int SLA(unsigned char* rom, int instruction_ptr, Opcode opcode);
int SRA(unsigned char* rom, int instruction_ptr, Opcode opcode);
int SRL(unsigned char* rom, int instruction_ptr, Opcode opcode);

// Bit operators
int Bit(unsigned char* rom, int instruction_ptr, Opcode opcode);
int Set(unsigned char* rom, int instruction_ptr, Opcode opcode);
int Res(unsigned char* rom, int instruction_ptr, Opcode opcode);

// Jumps
int Jump(unsigned char* rom, int instruction_ptr, Opcode opcode);
int JumpConditional(unsigned char* rom, int instruction_ptr, Opcode opcode);
int JumpHL(unsigned char* rom, int instruction_ptr, Opcode opcode);
int JumpRelative(unsigned char* rom, int instruction_ptr, Opcode opcode);
int JumpRelativeConditional(unsigned char* rom, int instruction_ptr, Opcode opcode);

// Calls
int Call(unsigned char* rom, int instruction_ptr, Opcode opcode);
int CallConditional(unsigned char* rom, int instruction_ptr, Opcode opcode);

// Restart
int Restart(unsigned char* rom, int instruction_ptr, Opcode opcode);

// Returns
int Return(unsigned char* rom, int instruction_ptr, Opcode opcode);
int ReturnConditional(unsigned char* rom, int instruction_ptr, Opcode opcode);
int ReturnInterrupt(unsigned char* rom, int instruction_ptr, Opcode opcode);

// 8-Bit Loads
int LoadN(unsigned char* rom, int instruction_ptr, Opcode opcode);
int LoadRR(unsigned char* rom, int instruction_ptr, Opcode opcode);
int LoadAN(unsigned char* rom, int instruction_ptr, Opcode opcode);
int LoadNA(unsigned char* rom, int instruction_ptr, Opcode opcode);
int LoadAC(unsigned char* rom, int instruction_ptr, Opcode opcode);
int LoadCA(unsigned char* rom, int instruction_ptr, Opcode opcode);
int LoadDecAHL(unsigned char* rom, int instruction_ptr, Opcode opcode);
int LoadDecHLA(unsigned char* rom, int instruction_ptr, Opcode opcode);
int LoadIncAHL(unsigned char* rom, int instruction_ptr, Opcode opcode);
int LoadIncHLA(unsigned char* rom, int instruction_ptr, Opcode opcode);
int LoadHNA(unsigned char* rom, int instruction_ptr, Opcode opcode);
int LoadHAN(unsigned char* rom, int instruction_ptr, Opcode opcode);

// 16-Bit Loads
int LoadNN(unsigned char* rom, int instruction_ptr, Opcode opcode);
int LoadSPHL(unsigned char* rom, int instruction_ptr, Opcode opcode);
int LoadHLSP(unsigned char* rom, int instruction_ptr, Opcode opcode);
int LoadNNSP(unsigned char* rom, int instruction_ptr, Opcode opcode);
int Push(unsigned char* rom, int instruction_ptr, Opcode opcode);
int Pop(unsigned char* rom, int instruction_ptr, Opcode opcode);

} // namespace back_end
} // namespace handlers

#endif // TURBO_SANTA_COMMON_BACK_END_OPCODE_HANDLERS_H_
