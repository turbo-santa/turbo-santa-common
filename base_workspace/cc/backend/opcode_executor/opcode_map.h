#ifndef TURBO_SANTA_COMMON_BACK_END_OPCODE_EXECUTOR_OPCODE_MAP_H_
#define TURBO_SANTA_COMMON_BACK_END_OPCODE_EXECUTOR_OPCODE_MAP_H_

#include <cstdint>
#include <vector>
#include <map>

#include "cc/backend/decompiler/instruction.h"
#include "cc/backend/opcode_executor/executor_context.h"
#include "cc/backend/opcode_executor/opcode_handlers.h"

namespace back_end {
namespace opcode_executor {

typedef std::function<int(const decompiler::Instruction&, ExecutorContext*)> OpcodeHandler;

std::map<uint16_t, OpcodeHandler> CreateOpcodeMap();

} // namespace opcodes
} // namespace back_end
#endif // TURBO_SANTA_COMMON_BACK_END_OPCODE_EXECUTOR_OPCODE_MAP_H_
