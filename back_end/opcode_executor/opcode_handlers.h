#ifndef TURBO_SANTA_COMMON_BACK_END_OPCODE_HANDLERS_H_
#define TURBO_SANTA_COMMON_BACK_END_OPCODE_HANDLERS_H_

#include <memory>

#include "back_end/memory/memory_mapper.h"
#include "back_end/opcode_executor/opcodes.h"
#include "back_end/opcode_executor/registers.h"
#include "back_end/opcode_executor/opcode_executor.h"

namespace back_end {
namespace handlers {


extern memory::MemoryMapper* mem_map;

// 8 Bit ALU
opcodes::OpcodeResult Add8Bit(unsigned char* rom, int instruction_ptr, opcodes::Opcode opcode);
opcodes::OpcodeResult Add8BitLiteral(unsigned char* rom, int instruction_ptr, opcodes::Opcode opcode);
opcodes::OpcodeResult ADC8Bit(unsigned char* rom, int instruction_ptr, opcodes::Opcode opcode);
opcodes::OpcodeResult ADC8BitLiteral(unsigned char* rom, int instruction_ptr, opcodes::Opcode opcode);
opcodes::OpcodeResult Sub8Bit(unsigned char* rom, int instruction_ptr, opcodes::Opcode opcode);
opcodes::OpcodeResult Sub8BitLiteral(unsigned char* rom, int instruction_ptr, opcodes::Opcode opcode);
opcodes::OpcodeResult SBC8Bit(unsigned char* rom, int instruction_ptr, Opcode opcode);
opcodes::OpcodeResult SBC8BitLiteral(unsigned char* rom, int instruction_ptr, Opcode opcode);
opcodes::OpcodeResult And8Bit(unsigned char* rom, int instruction_ptr, opcodes::Opcode opcode);
opcodes::OpcodeResult And8BitLiteral(unsigned char* rom, int instruction_ptr, opcodes::Opcode opcode);
opcodes::OpcodeResult Or8Bit(unsigned char* rom, int instruction_ptr, opcodes::Opcode opcode);
opcodes::OpcodeResult Or8BitLiteral(unsigned char* rom, int instruction_ptr, opcodes::Opcode opcode);
opcodes::OpcodeResult Xor8Bit(unsigned char* rom, int instruction_ptr, opcodes::Opcode opcode);
opcodes::OpcodeResult Xor8BitLiteral(unsigned char* rom, int instruction_ptr, opcodes::Opcode opcode);
opcodes::OpcodeResult Cp8Bit(unsigned char* rom, int instruction_ptr, opcodes::Opcode opcode);
opcodes::OpcodeResult Cp8BitLiteral(unsigned char* rom, int instruction_ptr, opcodes::Opcode opcode);
opcodes::OpcodeResult Inc8Bit(unsigned char* rom, int instruction_ptr, opcodes::Opcode opcode);
opcodes::OpcodeResult Dec8Bit(unsigned char* rom, int instruction_ptr, opcodes::Opcode opcode);

// 16 Bit ALU
opcodes::OpcodeResult Add16Bit(unsigned char* rom, int instruction_ptr, opcodes::Opcode opcode);
opcodes::OpcodeResult AddSPLiteral(unsigned char* rom, int instruction_ptr, opcodes::Opcode opcode);
opcodes::OpcodeResult Inc16Bit(unsigned char* rom, int instruction_ptr, opcodes::Opcode opcode);
opcodes::OpcodeResult Dec16Bit(unsigned char* rom, int instruction_ptr, opcodes::Opcode opcode);

// Miscelaneous
opcodes::OpcodeResult Swap(unsigned char* rom, int instruction_ptr, opcodes::Opcode opcode);
opcodes::OpcodeResult DAA(unsigned char* rom, int instruction_ptr, opcodes::Opcode opcode);
opcodes::OpcodeResult CPL(unsigned char* rom, int instruction_ptr, opcodes::Opcode opcode);
opcodes::OpcodeResult CCF(unsigned char* rom, int instruction_ptr, opcodes::Opcode opcode);
opcodes::OpcodeResult SCF(unsigned char* rom, int instruction_ptr, opcodes::Opcode opcode);
opcodes::OpcodeResult NOP(unsigned char* rom, int instruction_ptr, opcodes::Opcode opcode);
opcodes::OpcodeResult Halt(unsigned char* rom, int instruction_ptr, opcodes::Opcode opcode);
opcodes::OpcodeResult Stop(unsigned char* rom, int instruction_ptr, opcodes::Opcode opcode);
opcodes::OpcodeResult DI(unsigned char* rom, int instruction_ptr, opcodes::Opcode opcode);
opcodes::OpcodeResult EI(unsigned char* rom, int instruction_ptr, opcodes::Opcode opcode);

// Rotates & Shifts
opcodes::OpcodeResult RLCA(unsigned char* rom, int instruction_ptr, opcodes::Opcode opcode);
opcodes::OpcodeResult RLA(unsigned char* rom, int instruction_ptr, opcodes::Opcode opcode);
opcodes::OpcodeResult RRCA(unsigned char* rom, int instruction_ptr, opcodes::Opcode opcode);
opcodes::OpcodeResult RRA(unsigned char* rom, int instruction_ptr, opcodes::Opcode opcode);
opcodes::OpcodeResult RLC(unsigned char* rom, int instruction_ptr, opcodes::Opcode opcode);
opcodes::OpcodeResult RL(unsigned char* rom, int instruction_ptr, opcodes::Opcode opcode);
opcodes::OpcodeResult RRC(unsigned char* rom, int instruction_ptr, opcodes::Opcode opcode);
opcodes::OpcodeResult RR(unsigned char* rom, int instruction_ptr, opcodes::Opcode opcode);
opcodes::OpcodeResult SLA(unsigned char* rom, int instruction_ptr, opcodes::Opcode opcode);
opcodes::OpcodeResult SRA(unsigned char* rom, int instruction_ptr, opcodes::Opcode opcode);
opcodes::OpcodeResult SRL(unsigned char* rom, int instruction_ptr, opcodes::Opcode opcode);

// Bit operators
opcodes::OpcodeResult Bit(unsigned char* rom, int instruction_ptr, opcodes::Opcode opcode);
opcodes::OpcodeResult Set(unsigned char* rom, int instruction_ptr, opcodes::Opcode opcode);
opcodes::OpcodeResult Res(unsigned char* rom, int instruction_ptr, opcodes::Opcode opcode);

// Jumps
opcodes::OpcodeResult Jump(unsigned char* rom, int instruction_ptr, opcodes::Opcode opcode);
opcodes::OpcodeResult JumpConditional(unsigned char* rom, int instruction_ptr, opcodes::Opcode opcode);
opcodes::OpcodeResult JumpHL(unsigned char* rom, int instruction_ptr, opcodes::Opcode opcode);
opcodes::OpcodeResult JumpRelative(unsigned char* rom, int instruction_ptr, opcodes::Opcode opcode);
opcodes::OpcodeResult JumpRelativeConditional(unsigned char* rom, int instruction_ptr, opcodes::Opcode opcode);

// Calls
opcodes::OpcodeResult Call(unsigned char* rom, int instruction_ptr, opcodes::Opcode opcode);
opcodes::OpcodeResult CallConditional(unsigned char* rom, int instruction_ptr, opcodes::Opcode opcode);

// Restart
opcodes::OpcodeResult Restart(unsigned char* rom, int instruction_ptr, opcodes::Opcode opcode);

// Returns
opcodes::OpcodeResult Return(unsigned char* rom, int instruction_ptr, opcodes::Opcode opcode);
opcodes::OpcodeResult ReturnConditional(unsigned char* rom, int instruction_ptr, opcodes::Opcode opcode);
opcodes::OpcodeResult ReturnInterrupt(unsigned char* rom, int instruction_ptr, opcodes::Opcode opcode);

// 8-Bit Loads
opcodes::OpcodeResult LoadN(unsigned char* rom, int instruction_ptr, opcodes::Opcode opcode);
opcodes::OpcodeResult LoadRR(unsigned char* rom, int instruction_ptr, opcodes::Opcode opcode);
opcodes::OpcodeResult LoadAN(unsigned char* rom, int instruction_ptr, opcodes::Opcode opcode);
opcodes::OpcodeResult LoadNA(unsigned char* rom, int instruction_ptr, opcodes::Opcode opcode);
opcodes::OpcodeResult LoadAC(unsigned char* rom, int instruction_ptr, opcodes::Opcode opcode);
opcodes::OpcodeResult LoadCA(unsigned char* rom, int instruction_ptr, opcodes::Opcode opcode);
opcodes::OpcodeResult LoadDecAHL(unsigned char* rom, int instruction_ptr, opcodes::Opcode opcode);
opcodes::OpcodeResult LoadDecHLA(unsigned char* rom, int instruction_ptr, opcodes::Opcode opcode);
opcodes::OpcodeResult LoadIncAHL(unsigned char* rom, int instruction_ptr, opcodes::Opcode opcode);
opcodes::OpcodeResult LoadIncHLA(unsigned char* rom, int instruction_ptr, opcodes::Opcode opcode);
opcodes::OpcodeResult LoadHNA(unsigned char* rom, int instruction_ptr, opcodes::Opcode opcode);
opcodes::OpcodeResult LoadHAN(unsigned char* rom, int instruction_ptr, opcodes::Opcode opcode);

// 16-Bit Loads
opcodes::OpcodeResult LoadNN(unsigned char* rom, int instruction_ptr, opcodes::Opcode opcode);
opcodes::OpcodeResult LoadSPHL(unsigned char* rom, int instruction_ptr, opcodes::Opcode opcode);
opcodes::OpcodeResult LoadHLSP(unsigned char* rom, int instruction_ptr, opcodes::Opcode opcode);
opcodes::OpcodeResult LoadNNSP(unsigned char* rom, int instruction_ptr, opcodes::Opcode opcode);
opcodes::OpcodeResult Push(unsigned char* rom, int instruction_ptr, opcodes::Opcode opcode);
opcodes::OpcodeResult Pop(unsigned char* rom, int instruction_ptr, opcodes::Opcode opcode);

} // namespace back_end
} // namespace handlers

#endif // TURBO_SANTA_COMMON_BACK_END_OPCODE_HANDLERS_H_
