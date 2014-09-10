#ifndef TURBO_SANTA_COMMON_BACK_END_OPCODE_PARSER_H_
#define TURBO_SANTA_COMMON_BACK_END_OPCODE_PARSER_H_

#include <memory>

#include "back_end/memory/memory_mapper.h"
#include "back_end/opcode_handlers.h"
#include "back_end/opcodes.h"
#include "back_end/opcode_map.h"
#include "back_end/registers.h"

namespace test_harness {
class TestHarness;
} // namespace test_harness

namespace back_end {
namespace opcode_parser {

class OpcodeParser {
  public: 
    OpcodeParser(unsigned char* rom, long rom_size);
    void ReadInstruction();

  private:
    // TODO(Diego): It acutally starts at something like 0x100.
    int instruction_ptr_ = 0;
    unsigned char* rom_;

    friend class test_harness::TestHarness;
};

} // namespace opcode_parser
} // namespace back_end
#endif // TURBO_SANTA_COMMON_BACK_END_OPCODE_PARSER_H_
