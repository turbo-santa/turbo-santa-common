#include <vector>
#include "back_end/opcode_executor/opcode_handlers.h"
#include "back_end/opcode_executor/opcode_executor.h"
#include "back_end/opcode_executor/opcodes.h"
#include "test_harness/test_harness.h"
#include "test_harness/test_harness_utils.h"

#include "third_party/gtest/include/gtest/gtest.h"

namespace back_end {
namespace handlers {

unsigned char GetRegisterValue(unsigned char* rom, int instruction_ptr, unsigned char opcode);

using std::vector;
using handlers::OpcodeExecutor;
using Register = test_harness::RegisterNameValuePair;

// The fixture gets instantiated once per test case. We would like to reuse the
// OpcodeExecutor. Also, this will get cleaned up when the test is over.
OpcodeExecutor* parser = new OpcodeExecutor(nullptr, 0);

class OpcodeHandlersTest : public test_harness::TestHarness {
    protected:
        OpcodeHandlersTest() : test_harness::TestHarness(parser) {}
};

// Tests for ADD A,n

TEST_F(OpcodeHandlersTest, Add8BitAA) {
    SetRegisterState({{Register::A, 1}});
    EXPECT_EQ(0, get_instruction_ptr());
    ExecuteInstruction(0x87);
    EXPECT_EQ(1, get_instruction_ptr());
    EXPECT_REGISTER({{Register::A, 2}});
}

TEST_F(OpcodeHandlersTest, Add8BitAB) {
    SetRegisterState({{Register::A, 1}, {Register::B, 2}});
    EXPECT_EQ(0, get_instruction_ptr());
    ExecuteInstruction(0x80);
    EXPECT_EQ(1, get_instruction_ptr());
    EXPECT_REGISTER({{Register::A, 3}, {Register::B, 2}});
}

TEST_F(OpcodeHandlersTest, Add8BitAC) {
    SetRegisterState({{Register::A, 1}, {Register::C, 2}});
    EXPECT_EQ(0, get_instruction_ptr());
    ExecuteInstruction(0x81);
    EXPECT_EQ(1, get_instruction_ptr());
    EXPECT_REGISTER({{Register::A, 3}, {Register::C, 2}});
}

TEST_F(OpcodeHandlersTest, Add8BitAD) {
    SetRegisterState({{Register::A, 1}, {Register::D, 2}});
    EXPECT_EQ(0, get_instruction_ptr());
    ExecuteInstruction(0x82);
    EXPECT_EQ(1, get_instruction_ptr());
    EXPECT_REGISTER({{Register::A, 3}, {Register::D, 2}});
}

TEST_F(OpcodeHandlersTest, Add8BitAE) {
    SetRegisterState({{Register::A, 1}, {Register::E, 2}});
    EXPECT_EQ(0, get_instruction_ptr());
    ExecuteInstruction(0x83);
    EXPECT_EQ(1, get_instruction_ptr());
    EXPECT_REGISTER({{Register::A, 3}, {Register::E, 2}});
}

TEST_F(OpcodeHandlersTest, Add8BitAH) {
    SetRegisterState({{Register::A, 1}, {Register::H, 2}});
    EXPECT_EQ(0, get_instruction_ptr());
    ExecuteInstruction(0x84);
    EXPECT_EQ(1, get_instruction_ptr());
    EXPECT_REGISTER({{Register::A, 3}, {Register::H, 2}});
}

TEST_F(OpcodeHandlersTest, Add8BitAL) {
    SetRegisterState({{Register::A, 1}, {Register::L, 2}});
    EXPECT_EQ(0, get_instruction_ptr());
    ExecuteInstruction(0x85);
    EXPECT_EQ(1, get_instruction_ptr());
    EXPECT_REGISTER({{Register::A, 3}, {Register::L, 2}});
}

TEST_F(OpcodeHandlersTest, Add8BitAHL) {
    SetRegisterState({{Register::A, 1}, {Register::HL, 2}});
    EXPECT_EQ(0, get_instruction_ptr());
    ExecuteInstruction(0x86);
    EXPECT_EQ(1, get_instruction_ptr());
    EXPECT_REGISTER({{Register::A, 3}, {Register::HL, 2}});
}

TEST_F(OpcodeHandlersTest, Add8BitALiteral) {
    SetRegisterState({{Register::A, 1}});
    EXPECT_EQ(0, get_instruction_ptr());
    ExecuteInstruction(0xC6, static_cast<unsigned char>(5));
    EXPECT_EQ(2, get_instruction_ptr());
    EXPECT_REGISTER({{Register::A, 6}});
}

// Tests for ADC A,n
// Add n + Carry flag to A

TEST_F(OpcodeHandlersTest, Adc8BitAA) {
    SetRegisterState({{Register::A, 1}});
    EXPECT_EQ(0, get_instruction_ptr());
    ExecuteInstruction(0x8F);
    EXPECT_EQ(1, get_instruction_ptr());
    EXPECT_REGISTER({{Register::A, 2}});
}

TEST_F(OpcodeHandlersTest, Adc8BitAB) {
    SetRegisterState({{Register::A, 100}, {Register::B, 200}, {Register::FC, 0}});
    EXPECT_EQ(0, get_instruction_ptr());
    ExecuteInstruction(0x88);
    EXPECT_EQ(1, get_instruction_ptr());
    EXPECT_REGISTER({{Register::A, 44}, {Register::B, 200}, {Register::FC, 1}});
}

TEST_F(OpcodeHandlersTest, Adc8BitAC) {
    SetRegisterState({{Register::A, 100}, {Register::C, 200}, {Register::FC, 0}});
    EXPECT_EQ(0, get_instruction_ptr());
    ExecuteInstruction(0x89);
    EXPECT_EQ(1, get_instruction_ptr());
    EXPECT_REGISTER({{Register::A, 44}, {Register::C, 200}, {Register::FC, 1}});
}

TEST_F(OpcodeHandlersTest, Adc8BitAD) {
    SetRegisterState({{Register::A, 100}, {Register::D, 200}, {Register::FC, 0}});
    EXPECT_EQ(0, get_instruction_ptr());
    ExecuteInstruction(0x8A);
    EXPECT_EQ(1, get_instruction_ptr());
    EXPECT_REGISTER({{Register::A, 44}, {Register::D, 200}, {Register::FC, 1}});
}

TEST_F(OpcodeHandlersTest, Adc8BitAE) {
    SetRegisterState({{Register::A, 100}, {Register::E, 200}, {Register::FC, 0}});
    EXPECT_EQ(0, get_instruction_ptr());
    ExecuteInstruction(0x8B);
    EXPECT_EQ(1, get_instruction_ptr());
    EXPECT_REGISTER({{Register::A, 44}, {Register::E, 200}, {Register::FC, 1}});
}

TEST_F(OpcodeHandlersTest, Adc8BitAH) {
    SetRegisterState({{Register::A, 100}, {Register::H, 200}, {Register::FC, 0}});
    EXPECT_EQ(0, get_instruction_ptr());
    ExecuteInstruction(0x8C);
    EXPECT_EQ(1, get_instruction_ptr());
    EXPECT_REGISTER({{Register::A, 44}, {Register::H, 200}, {Register::FC, 1}});
}

TEST_F(OpcodeHandlersTest, Adc8BitAL) {
    SetRegisterState({{Register::A, 100}, {Register::L, 200}, {Register::FC, 0}});
    EXPECT_EQ(0, get_instruction_ptr());
    ExecuteInstruction(0x8D);
    EXPECT_EQ(1, get_instruction_ptr());
    EXPECT_REGISTER({{Register::A, 44}, {Register::L, 200}, {Register::FC, 1}});
}

TEST_F(OpcodeHandlersTest, Adc8BitAHL) {
    SetRegisterState({{Register::A, 100}, {Register::HL, 200}, {Register::FC, 0}});
    EXPECT_EQ(0, get_instruction_ptr());
    ExecuteInstruction(0x8E);
    EXPECT_EQ(1, get_instruction_ptr());
    EXPECT_REGISTER({{Register::A, 44}, {Register::HL, 200}, {Register::FC, 1}});
}

TEST_F(OpcodeHandlersTest, Adc8BitALiteral) {
    SetRegisterState({{Register::A, 100}, {Register::FC, 0}});
    EXPECT_EQ(0, get_instruction_ptr());
    ExecuteInstruction(0xCE, static_cast<unsigned char>(200));
    EXPECT_EQ(2, get_instruction_ptr());
    EXPECT_REGISTER({{Register::A, 44}, {Register::FC, 1}});
}

// Test SUB n
// Subtract n from A

TEST_F(OpcodeHandlersTest, Sub8BitA) {
    SetRegisterState({{Register::A, 2}});
    EXPECT_EQ(0, get_instruction_ptr());
    ExecuteInstruction(0x97);
    EXPECT_EQ(1, get_instruction_ptr());
    EXPECT_REGISTER({{Register::A, 0}});
}

TEST_F(OpcodeHandlersTest, Sub8BitB) {
    SetRegisterState({{Register::A, 2}, {Register::B, 1}});
    EXPECT_EQ(0, get_instruction_ptr());
    ExecuteInstruction(0x90);
    EXPECT_EQ(1, get_instruction_ptr());
    EXPECT_REGISTER({{Register::A, 1}, {Register::B, 1}});
}

TEST_F(OpcodeHandlersTest, Sub8BitC) {
    SetRegisterState({{Register::A, 2}, {Register::C, 1}});
    EXPECT_EQ(0, get_instruction_ptr());
    ExecuteInstruction(0x91);
    EXPECT_EQ(1, get_instruction_ptr());
    EXPECT_REGISTER({{Register::A, 1}, {Register::C, 1}});
}

TEST_F(OpcodeHandlersTest, Sub8BitD) {
    SetRegisterState({{Register::A, 2}, {Register::D, 1}});
    EXPECT_EQ(0, get_instruction_ptr());
    ExecuteInstruction(0x92);
    EXPECT_EQ(1, get_instruction_ptr());
    EXPECT_REGISTER({{Register::A, 1}, {Register::D, 1}});
}

TEST_F(OpcodeHandlersTest, Sub8BitE) {
    SetRegisterState({{Register::A, 2}, {Register::E, 1}});
    EXPECT_EQ(0, get_instruction_ptr());
    ExecuteInstruction(0x93);
    EXPECT_EQ(1, get_instruction_ptr());
    EXPECT_REGISTER({{Register::A, 1}, {Register::E, 1}});
}

TEST_F(OpcodeHandlersTest, Sub8BitH) {
    SetRegisterState({{Register::A, 2}, {Register::H, 1}});
    EXPECT_EQ(0, get_instruction_ptr());
    ExecuteInstruction(0x94);
    EXPECT_EQ(1, get_instruction_ptr());
    EXPECT_REGISTER({{Register::A, 1}, {Register::H, 1}});
}

TEST_F(OpcodeHandlersTest, Sub8BitL) {
    SetRegisterState({{Register::A, 2}, {Register::L, 1}});
    EXPECT_EQ(0, get_instruction_ptr());
    ExecuteInstruction(0x95);
    EXPECT_EQ(1, get_instruction_ptr());
    EXPECT_REGISTER({{Register::A, 1}, {Register::L, 1}});
}

TEST_F(OpcodeHandlersTest, Sub8BitHL) {
    SetRegisterState({{Register::A, 2}, {Register::HL, 1}});
    EXPECT_EQ(0, get_instruction_ptr());
    ExecuteInstruction(0x96);
    EXPECT_EQ(1, get_instruction_ptr());
    EXPECT_REGISTER({{Register::A, 1}, {Register::HL, 1}});
}

TEST_F(OpcodeHandlersTest, Sub8BitLiteral) {
    SetRegisterState({{Register::A, 2}});
    EXPECT_EQ(0, get_instruction_ptr());
    ExecuteInstruction(0xD6, static_cast<unsigned char>(1));
    EXPECT_EQ(2, get_instruction_ptr());
    EXPECT_REGISTER({{Register::A, 1}});
}


// Test SBC A,n
// Subtract n + carry flag from A

TEST_F(OpcodeHandlersTest, Sbc8BitA) {
    SetRegisterState({{Register::A, 2}});
    EXPECT_EQ(0, get_instruction_ptr());
    ExecuteInstruction(0x9F);
    EXPECT_EQ(1, get_instruction_ptr());
    EXPECT_REGISTER({{Register::A, 0}, {Register::FC, 0}});
}

TEST_F(OpcodeHandlersTest, Cp8Bit) {
    SetRegisterState({{Register::A, 1}, {Register::B, 1}});
    ExecuteInstruction(0xb8);
    EXPECT_REGISTER({{Register::FZ, 1}, {Register::FN, 1}, {Register::FH, 0}, {Register::FC, 0}});

    SetRegisterState({{Register::A, 1}, {Register::B, 2}});
    ExecuteInstruction(0xb8);
    EXPECT_REGISTER({{Register::FZ, 0}, {Register::FN, 1}, {Register::FH, 1}, {Register::FC, 1}});
}

TEST_F(OpcodeHandlersTest, And8Bit) {
    SetRegisterState({{Register::A, 1}, {Register::B, 2}});
    ExecuteInstruction(0xA0);
    EXPECT_REGISTER({{Register::A, 0}, {Register::B, 2}});

    SetRegisterState({{Register::A, 2}, {Register::B, 2}});
    ExecuteInstruction(0xA0);
    EXPECT_REGISTER({{Register::A, 2}, {Register::B, 2}});

    SetRegisterState({{Register::A, 0}, {Register::B, 0}});
    ExecuteInstruction(0xA0);
    EXPECT_REGISTER({{Register::A, 0}, {Register::B, 0}});
}

TEST_F(OpcodeHandlersTest, Or8Bit) {
    SetRegisterState({{Register::A, 1}, {Register::B, 2}});
    ExecuteInstruction(0xB0);
    EXPECT_REGISTER({{Register::A, 3}, {Register::B, 2}});

    SetRegisterState({{Register::A, 2}, {Register::B, 2}});
    ExecuteInstruction(0xB0);
    EXPECT_REGISTER({{Register::A, 2}, {Register::B, 2}});

    SetRegisterState({{Register::A, 1}, {Register::B, 0}});
    ExecuteInstruction(0xB0);
    EXPECT_REGISTER({{Register::A, 1}, {Register::B, 0}});
}

TEST_F(OpcodeHandlersTest, Xor8Bit) {
    SetRegisterState({{Register::A, 1}, {Register::B, 2}});
    ExecuteInstruction(0xA8);
    EXPECT_REGISTER({{Register::A, 3}, {Register::B, 2}});

    SetRegisterState({{Register::A, 2}, {Register::B, 2}});
    ExecuteInstruction(0xA8);
    EXPECT_REGISTER({{Register::A, 0}, {Register::B, 2}});

    SetRegisterState({{Register::A, 0}, {Register::B, 0}});
    ExecuteInstruction(0xA8);
    EXPECT_REGISTER({{Register::A, 0}, {Register::B, 0}});
}

TEST_F(OpcodeHandlersTest, INC8Bit) {
    SetRegisterState({{Register::A, 1}});
    ExecuteInstruction(0x3C);
    EXPECT_REGISTER({{Register::A, 2}});
}

TEST_F(OpcodeHandlersTest, Dec8Bit) {
    SetRegisterState({{Register::A, 1}});
    ExecuteInstruction(0x3D);
    EXPECT_REGISTER({{Register::A, 0}});
}

TEST_F(OpcodeHandlersTest, LoadDecHLA) {
  SetRegisterState({{Register::A, 0xab}, {Register::HL, 0xa001}});
  ExecuteInstruction(0x32);
  EXPECT_REGISTER({{Register::A, 0xab}, {Register::HL, 0xa000}});
  SetRegisterState({{Register::A, 0}, {Register::HL, 0xa001}});
  ExecuteInstruction(0x2a);
  EXPECT_REGISTER({{Register::A, 0xab}, {Register::HL, 0xa002}});
}

} // namespace handlers
} // namespace back_end
