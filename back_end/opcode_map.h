#ifndef TURBO_SANTA_COMMON_BACK_END_OPCODE_MAP_H_
#define TURBO_SANTA_COMMON_BACK_END_OPCODE_MAP_H_

#include <vector>
#include <map>

#include "back_end/opcodes.h"
#include "back_end/opcode_handlers.h"
#include "back_end/registers.h"

namespace back_end {
namespace opcodes {

extern unsigned char* rA; 
extern unsigned char* rB; 
extern unsigned char* rC; 
extern unsigned char* rD; 
extern unsigned char* rE; 
extern unsigned char* rF; 
extern unsigned char* rH; 
extern unsigned char* rL; 
extern unsigned char* rAF;
extern unsigned char* rBC;
extern unsigned char* rDE;
extern unsigned char* rHL;
extern unsigned char* rPC;
extern unsigned char* rSP;

extern std::map<unsigned short, Opcode> opcode_map;

} // namespace opcodes
} // namespace back_end
#endif // TURBO_SANTA_COMMON_BACK_END_OPCODE_MAP_H_
