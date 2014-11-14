#ifndef TURBO_SANTA_COMMON_BACK_END_OPCODE_PARSER_H_
#define TURBO_SANTA_COMMON_BACK_END_OPCODE_PARSER_H_

#include <memory>

#include "back_end/memory/memory_mapper.h"
#include "back_end/opcode_executor/opcode_handlers.h"
#include "back_end/opcode_executor/opcodes.h"
#include "back_end/opcode_executor/opcode_map.h"
#include "back_end/opcode_executor/registers.h"

namespace test_harness {
class TestHarness;
} // namespace test_harness

namespace back_end {
namespace clocktroller {
class ClocktrollerTest;
} // namespace clocktroller
} // namespace back_end

namespace back_end {
namespace handlers {

class OpcodeExecutor {
  public: 
    OpcodeExecutor(unsigned char* rom, unsigned long rom_size);
    unsigned int ReadInstruction();

  private:
    // TODO(Diego): It acutally starts at something like 0x100.
    int instruction_ptr_ = 0;
    unsigned char* rom_;

    friend class test_harness::TestHarness;
    friend class back_end::clocktroller::ClocktrollerTest;
};

} // namespace handlers
} // namespace back_end
#endif // TURBO_SANTA_COMMON_BACK_END_OPCODE_PARSER_H_
