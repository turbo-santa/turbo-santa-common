#ifndef TURBO_SANTA_COMMON_BACK_END_OPCODE_MAP_H_
#define TURBO_SANTA_COMMON_BACK_END_OPCODE_MAP_H_

#include <vector>
#include "back_end/opcodes.h"
#include "back_end/opcode_handlers.h"

namespace back_end {
std::vector<Opcode> opcodes = {
    {0x87, 4, Add8Bit}
};
} // namespace back_end
