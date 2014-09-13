#include "back_end/opcode_handlers.h"
#include "back_end/opcode_parser.h"
#include "back_end/opcodes.h"
#include "test_harness/test_harness.h"
#include "test_harness/test_harness_utils.h"

#include "third_party/gtest/include/gtest/gtest.h"

namespace back_end {
namespace handlers {

unsigned char GetRegisterValue(unsigned char* rom, int instruction_ptr, unsigned char opcode);

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
    EXPECT_EQ(0, get_instruction_ptr());
    ExecuteInstruction(0x80);
    EXPECT_EQ(1, get_instruction_ptr());
    AssertRegisterState({{Register::A, 3}, {Register::B, 2}});
    EXPECT_EQ(1, get_instruction_ptr());
}

TEST_F(OpcodeHandlersTest, Add8BitDirect) {
    EXPECT_EQ(0, get_instruction_ptr());
    AssertRegisterState({{Register::A, 0}, {Register::B, 0}});
    SetRegisterState({{Register::A, 1}, {Register::B, 2}});
    AssertRegisterState({{Register::A, 1}, {Register::B, 2}});

    get_rom_ptr()[get_instruction_ptr()] = 0x80;
    int actual_new_instruction_ptr = Add8Bit(get_rom_ptr(), get_instruction_ptr(), {0x80, nullptr, nullptr, nullptr});
    int expected_new_instruction_ptr = 1;
    EXPECT_EQ(expected_new_instruction_ptr, actual_new_instruction_ptr);
    AssertRegisterState({{Register::A, 3}, {Register::B, 2}});
}

TEST_F(OpcodeHandlersTest, GetRegisterValue) {
    get_rom_ptr()[get_instruction_ptr()] = 0x80;
    SetRegisterState({{Register::B, 1}});

    int expected_register_value = 1;
    int actual_register_value = GetRegisterValue(get_rom_ptr(), get_instruction_ptr(), 0x80);
    EXPECT_EQ(expected_register_value, actual_register_value);
}

} // namespace handlers
} // namespace back_end
