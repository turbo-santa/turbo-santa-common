#ifndef TURBO_SANTA_COMMON_BACK_END_OPCODE_MAP_H_
#define TURBO_SANTA_COMMON_BACK_END_OPCODE_MAP_H_

#include <vector>
#include <map>

#include "back_end/opcodes.h"
#include "back_end/opcode_handlers.h"
#include "back_end/registers.h"

namespace back_end {
namespace opcodes {

using namespace handlers;

unsigned char* rA = &cpu.flag_struct.rA;
unsigned char* rB = &cpu.bc_struct.rB;
unsigned char* rC = &cpu.bc_struct.rC;
unsigned char* rD = &cpu.de_struct.rD;
unsigned char* rE = &cpu.de_struct.rE;
unsigned char* rF = (unsigned char*) &cpu.flag_struct.rF;
unsigned char* rH = &cpu.hl_struct.rH;
unsigned char* rL = &cpu.hl_struct.rL;
unsigned char* rAF = (unsigned char*) &cpu.rAF;
unsigned char* rBC = (unsigned char*) &cpu.rBC;
unsigned char* rDE = (unsigned char*) &cpu.rDE;
unsigned char* rHL = (unsigned char*) &cpu.rHL;
unsigned char* rPC = (unsigned char*) &cpu.rPC;
unsigned char* rSP = (unsigned char*) &cpu.rSP;

extern std::map<unsigned char, Opcode> opcode_map;

} // namespace opcodes
} // namespace back_end
#endif // TURBO_SANTA_COMMON_BACK_END_OPCODE_MAP_H_
