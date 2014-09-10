#include "back_end/opcode_handlers.h"
#include "back_end/opcode_parser.h"
#include "back_end/opcodes.h"
#include "test_harness/test_harness.h"
#include "test_harness/test_harness_utils.h"

#include "third_party/gtest/include/gtest/gtest.h"

namespace back_end {
namespace handlers {

using opcode_parser::OpcodeParser;
using Register = test_harness::RegisterNameValuePair;

// The fixture gets instantiated once per test case. We would like to reuse the
// OpcodeParser. Also, this will get cleaned up when the test is over.
OpcodeParser* parser = new OpcodeParser(nullptr, 0);

class OpcodeHandlersTest : public test_harness::TestHarness {
    protected:
        OpcodeHandlersTest() : test_harness::TestHarness(parser) {}
};

TEST_F(OpcodeHandlersTest, Add8Bit) {
    SetRegisterState({{Register::A, 1}, {Register::B, 2}});
    ExecuteInstruction(0x80);
    AssertRegisterState({{Register::A, 3}, {Register::B, 2}});
}

} // namespace handlers
} // namespace back_end
