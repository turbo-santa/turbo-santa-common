#ifndef TURBO_SANTA_COMMON_BACK_END_OPCODE_MAP_H_
#define TURBO_SANTA_COMMON_BACK_END_OPCODE_MAP_H_

#include <vector>
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

std::vector<std::vector<Opcode>> opcodes = {
    ToList1({{0x06, rB},
            {0x0e, rC},
            {0x16, rD},
            {0x1e, rE},
            {0x26, rH},
            {0x2e, rL}}, LoadNN),
    ToList2({{0x7f, rA, rA},
            {0x78, rA, rB},
            {0x79, rA, rC},
            {0x7a, rA, rD},
            {0x7b, rA, rE},
            {0x7c, rA, rH},
            {0x7d, rA, rL},
            {0x7e, rA, rHL},
            {0x40, rB, rB},
            {0x41, rB, rC},
            {0x42, rB, rD},
            {0x43, rB, rE},
            {0x44, rB, rH},
            {0x45, rB, rL},
            {0x46, rB, rHL},
            {0x48, rC, rB},
            {0x49, rC, rC},
            {0x4a, rC, rD},
            {0x4b, rC, rE},
            {0x4c, rC, rH},
            {0x4d, rC, rL},
            {0x4e, rC, rHL},
            {0x50, rD, rB},
            {0x51, rD, rC},
            {0x52, rD, rD},
            {0x53, rD, rE},
            {0x54, rD, rH},
            {0x55, rD, rL},
            {0x56, rD, rHL},
            {0x58, rE, rB},
            {0x59, rE, rC},
            {0x5a, rE, rD},
            {0x5b, rE, rE},
            {0x5c, rE, rH},
            {0x5d, rE, rL},
            {0x5e, rE, rHL},
            {0x60, rH, rB},
            {0x61, rH, rC},
            {0x62, rH, rD},
            {0x63, rH, rE},
            {0x64, rH, rH},
            {0x65, rH, rL},
            {0x66, rH, rHL},
            {0x68, rL, rB},
            {0x69, rL, rC},
            {0x6a, rL, rD},
            {0x6b, rL, rE},
            {0x6c, rL, rH},
            {0x6d, rL, rL},
            {0x6e, rL, rHL},
            {0x70, rHL, rB},
            {0x71, rHL, rC},
            {0x72, rHL, rD},
            {0x73, rHL, rE},
            {0x74, rHL, rH},
            {0x75, rHL, rL}
            /* {0x36, rHL, n} */}, LoadRR) };

} // namespace opcodes
} // namespace back_end
#endif // TURBO_SANTA_COMMON_BACK_END_OPCODE_MAP_H_
