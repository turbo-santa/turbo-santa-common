#ifndef TURBO_SANTA_COMMON_BACK_END_OPCODE_HANDLERS_H_
#define TURBO_SANTA_COMMON_BACK_END_OPCODE_HANDLERS_H_

#include <memory>

#include "back_end/memory/memory_mapper.h"
#include "back_end/opcode_executor/opcodes.h"
#include "back_end/opcode_executor/registers.h"

namespace back_end {
namespace handlers {

// Helper functions.
void PushRegister(memory::MemoryMapper* memory_mapper,
                  registers::GB_CPU* cpu, unsigned short* reg);

// 8 Bit ALU
int Add8Bit(handlers::ExecutorContext* context);
int Add8BitLiteral(handlers::ExecutorContext* context);
int ADC8Bit(handlers::ExecutorContext* context);
int ADC8BitLiteral(handlers::ExecutorContext* context);
int Sub8Bit(handlers::ExecutorContext* context);
int Sub8BitLiteral(handlers::ExecutorContext* context);
int SBC8Bit(handlers::ExecutorContext* context);
int SBC8BitLiteral(handlers::ExecutorContext* context);
int And8Bit(handlers::ExecutorContext* context);
int And8BitLiteral(handlers::ExecutorContext* context);
int Or8Bit(handlers::ExecutorContext* context);
int Or8BitLiteral(handlers::ExecutorContext* context);
int Xor8Bit(handlers::ExecutorContext* context);
int Xor8BitLiteral(handlers::ExecutorContext* context);
int Cp8Bit(handlers::ExecutorContext* context);
int Cp8BitLiteral(handlers::ExecutorContext* context);
int Inc8Bit(handlers::ExecutorContext* context);
int Dec8Bit(handlers::ExecutorContext* context);

// 16 Bit ALU
int Add16Bit(handlers::ExecutorContext* context);
int AddSPLiteral(handlers::ExecutorContext* context);
int Inc16Bit(handlers::ExecutorContext* context);
int Dec16Bit(handlers::ExecutorContext* context);

// Miscelaneous
int Swap(handlers::ExecutorContext* context);
int DAA(handlers::ExecutorContext* context);
int CPL(handlers::ExecutorContext* context);
int CCF(handlers::ExecutorContext* context);
int SCF(handlers::ExecutorContext* context);
int NOP(handlers::ExecutorContext* context);
int Halt(handlers::ExecutorContext* context);
int Stop(handlers::ExecutorContext* context);
int DI(handlers::ExecutorContext* context);
int EI(handlers::ExecutorContext* context);

// Rotates & Shifts
int RLCA(handlers::ExecutorContext* context);
int RLA(handlers::ExecutorContext* context);
int RRCA(handlers::ExecutorContext* context);
int RRA(handlers::ExecutorContext* context);
int RLC(handlers::ExecutorContext* context);
int RL(handlers::ExecutorContext* context);
int RRC(handlers::ExecutorContext* context);
int RR(handlers::ExecutorContext* context);
int SLA(handlers::ExecutorContext* context);
int SRA(handlers::ExecutorContext* context);
int SRL(handlers::ExecutorContext* context);

// Bit operators
int Bit(handlers::ExecutorContext* context);
int Set(handlers::ExecutorContext* context);
int Res(handlers::ExecutorContext* context);

// Jumps
int Jump(handlers::ExecutorContext* context);
int JumpConditional(handlers::ExecutorContext* context);
int JumpHL(handlers::ExecutorContext* context);
int JumpRelative(handlers::ExecutorContext* context);
int JumpRelativeConditional(handlers::ExecutorContext* context);

// Calls
int Call(handlers::ExecutorContext* context);
int CallConditional(handlers::ExecutorContext* context);

// Restart
int Restart(handlers::ExecutorContext* context);

// Returns
int Return(handlers::ExecutorContext* context);
int ReturnConditional(handlers::ExecutorContext* context);
int ReturnInterrupt(handlers::ExecutorContext* context);

// 8-Bit Loads
int LoadN(handlers::ExecutorContext* context);
int LoadRR(handlers::ExecutorContext* context);
int LoadAN(handlers::ExecutorContext* context);
int LoadNA(handlers::ExecutorContext* context);
int LoadAC(handlers::ExecutorContext* context);
int LoadCA(handlers::ExecutorContext* context);
int LoadDecAHL(handlers::ExecutorContext* context);
int LoadDecHLA(handlers::ExecutorContext* context);
int LoadIncAHL(handlers::ExecutorContext* context);
int LoadIncHLA(handlers::ExecutorContext* context);
int LoadHNA(handlers::ExecutorContext* context);
int LoadHAN(handlers::ExecutorContext* context);

// 16-Bit Loads
int LoadNN(handlers::ExecutorContext* context);
int LoadSPHL(handlers::ExecutorContext* context);
int LoadHLSP(handlers::ExecutorContext* context);
int LoadNNSP(handlers::ExecutorContext* context);
int Push(handlers::ExecutorContext* context);
int Pop(handlers::ExecutorContext* context);

} // namespace back_end
} // namespace handlers

#endif // TURBO_SANTA_COMMON_BACK_END_OPCODE_HANDLERS_H_
