#ifndef TURBO_SANTA_COMMON_BACK_END_OPCODE_EXECUTOR_OPCODE_HANDLERS_H_
#define TURBO_SANTA_COMMON_BACK_END_OPCODE_EXECUTOR_OPCODE_HANDLERS_H_

#include <memory>

#include "backend/decompiler/instruction.h"
#include "backend/memory/memory_mapper.h"
#include "backend/opcode_executor/executor_context.h"
#include "backend/opcode_executor/registers.h"

namespace back_end {
namespace opcode_executor {

// Helper functions.
void PushRegister(memory::MemoryMapper* memory_mapper,
                  registers::GB_CPU* cpu, unsigned short* reg);

// 8 Bit ALU
int Add8Bit(const decompiler::Instruction& instruction, ExecutorContext* context);
int Add8BitAddress(const decompiler::Instruction& instruction, ExecutorContext* context);
int Add8BitLiteral(const decompiler::Instruction& instruction, ExecutorContext* context);
int ADC8Bit(const decompiler::Instruction& instruction, ExecutorContext* context);
int ADC8BitAddress(const decompiler::Instruction& instruction, ExecutorContext* context);
int ADC8BitLiteral(const decompiler::Instruction& instruction, ExecutorContext* context);
int Sub8Bit(const decompiler::Instruction& instruction, ExecutorContext* context);
int Sub8BitAddress(const decompiler::Instruction& instruction, ExecutorContext* context);
int Sub8BitLiteral(const decompiler::Instruction& instruction, ExecutorContext* context);
int SBC8Bit(const decompiler::Instruction& instruction, ExecutorContext* context);
int SBC8BitAddress(const decompiler::Instruction& instruction, ExecutorContext* context);
int SBC8BitLiteral(const decompiler::Instruction& instruction, ExecutorContext* context);
int And8Bit(const decompiler::Instruction& instruction, ExecutorContext* context);
int And8BitAddress(const decompiler::Instruction& instruction, ExecutorContext* context);
int And8BitLiteral(const decompiler::Instruction& instruction, ExecutorContext* context);
int Or8Bit(const decompiler::Instruction& instruction, ExecutorContext* context);
int Or8BitAddress(const decompiler::Instruction& instruction, ExecutorContext* context);
int Or8BitLiteral(const decompiler::Instruction& instruction, ExecutorContext* context);
int Xor8Bit(const decompiler::Instruction& instruction, ExecutorContext* context);
int Xor8BitAddress(const decompiler::Instruction& instruction, ExecutorContext* context);
int Xor8BitLiteral(const decompiler::Instruction& instruction, ExecutorContext* context);
int Cp8Bit(const decompiler::Instruction& instruction, ExecutorContext* context);
int Cp8BitAddress(const decompiler::Instruction& instruction, ExecutorContext* context);
int Cp8BitLiteral(const decompiler::Instruction& instruction, ExecutorContext* context);
int Inc8Bit(const decompiler::Instruction& instruction, ExecutorContext* context);
int Inc8BitAddress(const decompiler::Instruction& instruction, ExecutorContext* context);
int Dec8Bit(const decompiler::Instruction& instruction, ExecutorContext* context);
int Dec8BitAddress(const decompiler::Instruction& instruction, ExecutorContext* context);
  
// 16 Bit ALU
int Add16Bit(const decompiler::Instruction& instruction, ExecutorContext* context);
int AddSPLiteral(const decompiler::Instruction& instruction, ExecutorContext* context);
int Inc16Bit(const decompiler::Instruction& instruction, ExecutorContext* context);
int Dec16Bit(const decompiler::Instruction& instruction, ExecutorContext* context);

// Miscelaneous
int Swap(const decompiler::Instruction& instruction, ExecutorContext* context);
int SwapAddress(const decompiler::Instruction& instruction, ExecutorContext* context);
int DAA(const decompiler::Instruction& instruction, ExecutorContext* context);
int CPL(const decompiler::Instruction& instruction, ExecutorContext* context);
int CCF(const decompiler::Instruction& instruction, ExecutorContext* context);
int SCF(const decompiler::Instruction& instruction, ExecutorContext* context);
int NOP(const decompiler::Instruction& instruction, ExecutorContext* context);
int Halt(const decompiler::Instruction& instruction, ExecutorContext* context);
int Stop(const decompiler::Instruction& instruction, ExecutorContext* context);
int DI(const decompiler::Instruction& instruction, ExecutorContext* context);
int EI(const decompiler::Instruction& instruction, ExecutorContext* context);

// Rotates & Shifts
int RLCA(const decompiler::Instruction& instruction, ExecutorContext* context);
int RLA(const decompiler::Instruction& instruction, ExecutorContext* context);
int RRCA(const decompiler::Instruction& instruction, ExecutorContext* context);
int RRA(const decompiler::Instruction& instruction, ExecutorContext* context);
int RLC(const decompiler::Instruction& instruction, ExecutorContext* context);
int RLCAddress(const decompiler::Instruction& instruction, ExecutorContext* context);
int RL(const decompiler::Instruction& instruction, ExecutorContext* context);
int RLAddress(const decompiler::Instruction& instruction, ExecutorContext* context);
int RRC(const decompiler::Instruction& instruction, ExecutorContext* context);
int RRCAddress(const decompiler::Instruction& instruction, ExecutorContext* context);
int RR(const decompiler::Instruction& instruction, ExecutorContext* context);
int RRAddress(const decompiler::Instruction& instruction, ExecutorContext* context);
int SLA(const decompiler::Instruction& instruction, ExecutorContext* context);
int SLAAddress(const decompiler::Instruction& instruction, ExecutorContext* context);
int SRA(const decompiler::Instruction& instruction, ExecutorContext* context);
int SRAAddress(const decompiler::Instruction& instruction, ExecutorContext* context);
int SRL(const decompiler::Instruction& instruction, ExecutorContext* context);
int SRLAddress(const decompiler::Instruction& instruction, ExecutorContext* context);

// Bit operators
int Bit(const decompiler::Instruction& instruction, ExecutorContext* context);
int BitAddress(const decompiler::Instruction& instruction, ExecutorContext* context);
int Set(const decompiler::Instruction& instruction, ExecutorContext* context);
int Res(const decompiler::Instruction& instruction, ExecutorContext* context);

// Jumps
int Jump(const decompiler::Instruction& instruction, ExecutorContext* context);
int JumpConditional(const decompiler::Instruction& instruction, ExecutorContext* context);
int JumpHL(const decompiler::Instruction& instruction, ExecutorContext* context);
int JumpRelative(const decompiler::Instruction& instruction, ExecutorContext* context);
int JumpRelativeConditional(const decompiler::Instruction& instruction, ExecutorContext* context);

// Calls
int Call(const decompiler::Instruction& instruction, ExecutorContext* context);
int CallConditional(const decompiler::Instruction& instruction, ExecutorContext* context);

// Restart
int Restart(const decompiler::Instruction& instruction, ExecutorContext* context);

// Returns
int Return(const decompiler::Instruction& instruction, ExecutorContext* context);
int ReturnConditional(const decompiler::Instruction& instruction, ExecutorContext* context);
int ReturnInterrupt(const decompiler::Instruction& instruction, ExecutorContext* context);

// 8-Bit Loads
int LoadN(const decompiler::Instruction& instruction, ExecutorContext* context);
int LoadRR8Bit(const decompiler::Instruction& instruction, ExecutorContext* context);
int LoadRR8BitAddress(const decompiler::Instruction& instruction, ExecutorContext* context);
int LoadRR8BitIntoAddress(const decompiler::Instruction& instruction, ExecutorContext* context);
int Load8BitLiteral(const decompiler::Instruction& instruction, ExecutorContext* context);
int LoadRR16Bit(const decompiler::Instruction& instruction, ExecutorContext* context);
int LoadAN(const decompiler::Instruction& instruction, ExecutorContext* context);
int LoadAN16BitLiteral(const decompiler::Instruction& instruction, ExecutorContext* context);
int LoadAN8BitLiteral(const decompiler::Instruction& instruction, ExecutorContext* context);
int LoadNA(const decompiler::Instruction& instruction, ExecutorContext* context);
int LoadNAAddress(const decompiler::Instruction& instruction, ExecutorContext* context);
int LoadNA16BitLiteral(const decompiler::Instruction& instruction, ExecutorContext* context);
int LoadAC(const decompiler::Instruction& instruction, ExecutorContext* context);
int LoadCA(const decompiler::Instruction& instruction, ExecutorContext* context);
int LoadDecAHL(const decompiler::Instruction& instruction, ExecutorContext* context);
int LoadDecHLA(const decompiler::Instruction& instruction, ExecutorContext* context);
int LoadIncAHL(const decompiler::Instruction& instruction, ExecutorContext* context);
int LoadIncHLA(const decompiler::Instruction& instruction, ExecutorContext* context);
int LoadHNA(const decompiler::Instruction& instruction, ExecutorContext* context);
int LoadHAN(const decompiler::Instruction& instruction, ExecutorContext* context);

// 16-Bit Loads
int LoadNN(const decompiler::Instruction& instruction, ExecutorContext* context);
int LoadSPHL(const decompiler::Instruction& instruction, ExecutorContext* context);
int LoadHLSP(const decompiler::Instruction& instruction, ExecutorContext* context);
int LoadNNSP(const decompiler::Instruction& instruction, ExecutorContext* context);
int Push(const decompiler::Instruction& instruction, ExecutorContext* context);
int Pop(const decompiler::Instruction& instruction, ExecutorContext* context);

// Bonus
int HaltAndCatchFire(const decompiler::Instruction& instruction, ExecutorContext* context);

} // namespace back_end
} // namespace opcode_executor

#endif // TURBO_SANTA_COMMON_BACK_END_OPCODE_EXECUTOR_OPCODE_HANDLERS_H_
