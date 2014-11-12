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

// Tests for 8-bit ALU

// Tests for ADD A,n

TEST_F(OpcodeHandlersTest, Add8BitAA) {
    SetRegisterState({{Register::A, 1}});
    EXPECT_EQ(0, get_instruction_ptr());
    EXPECT_EQ(4, ExecuteInstruction(0x87));
    EXPECT_EQ(1, get_instruction_ptr());
    EXPECT_REGISTER({{Register::A, 2}});
}

TEST_F(OpcodeHandlersTest, Add8BitAB) {
    SetRegisterState({{Register::A, 1}, {Register::B, 2}});
    EXPECT_EQ(0, get_instruction_ptr());
    EXPECT_EQ(4, ExecuteInstruction(0x80));
    EXPECT_EQ(1, get_instruction_ptr());
    EXPECT_REGISTER({{Register::A, 3}, {Register::B, 2}});
}

TEST_F(OpcodeHandlersTest, Add8BitAC) {
    SetRegisterState({{Register::A, 100}, {Register::C, 200}, {Register::FC, 0}});
    EXPECT_EQ(0, get_instruction_ptr());
    EXPECT_EQ(4, ExecuteInstruction(0x81));
    EXPECT_EQ(1, get_instruction_ptr());
    EXPECT_REGISTER({{Register::A, 44}, {Register::C, 200}, {Register::FC, 1}});
}

TEST_F(OpcodeHandlersTest, Add8BitAD) {
    SetRegisterState({{Register::A, 1}, {Register::D, 2}});
    EXPECT_EQ(0, get_instruction_ptr());
    EXPECT_EQ(4, ExecuteInstruction(0x82));
    EXPECT_EQ(1, get_instruction_ptr());
    EXPECT_REGISTER({{Register::A, 3}, {Register::D, 2}});
}

TEST_F(OpcodeHandlersTest, Add8BitAE) {
    SetRegisterState({{Register::A, 1}, {Register::E, 2}});
    EXPECT_EQ(0, get_instruction_ptr());
    EXPECT_EQ(4, ExecuteInstruction(0x83));
    EXPECT_EQ(1, get_instruction_ptr());
    EXPECT_REGISTER({{Register::A, 3}, {Register::E, 2}});
}

TEST_F(OpcodeHandlersTest, Add8BitAH) {
    SetRegisterState({{Register::A, 1}, {Register::H, 2}});
    EXPECT_EQ(0, get_instruction_ptr());
    EXPECT_EQ(4, ExecuteInstruction(0x84));
    EXPECT_EQ(1, get_instruction_ptr());
    EXPECT_REGISTER({{Register::A, 3}, {Register::H, 2}});
}

TEST_F(OpcodeHandlersTest, Add8BitAL) {
    SetRegisterState({{Register::A, 1}, {Register::L, 2}});
    EXPECT_EQ(0, get_instruction_ptr());
    EXPECT_EQ(4, ExecuteInstruction(0x85));
    EXPECT_EQ(1, get_instruction_ptr());
    EXPECT_REGISTER({{Register::A, 3}, {Register::L, 2}});
}

TEST_F(OpcodeHandlersTest, Add8BitAHL) {
    SetRegisterState({{Register::A, 1}, {Register::HL, 2}});
    EXPECT_EQ(0, get_instruction_ptr());
    EXPECT_EQ(8, ExecuteInstruction(0x86));
    EXPECT_EQ(1, get_instruction_ptr());
    EXPECT_REGISTER({{Register::A, 3}, {Register::HL, 2}});
}

TEST_F(OpcodeHandlersTest, Add8BitALiteral) {
    SetRegisterState({{Register::A, 1}});
    EXPECT_EQ(0, get_instruction_ptr());
    EXPECT_EQ(8, ExecuteInstruction(0xC6, static_cast<unsigned char>(5)));
    EXPECT_EQ(2, get_instruction_ptr());
    EXPECT_REGISTER({{Register::A, 6}});
}

// Tests for ADC A,n
// Add n + Carry flag to A

TEST_F(OpcodeHandlersTest, Adc8BitAA) {
    SetRegisterState({{Register::A, 1}});
    EXPECT_EQ(0, get_instruction_ptr());
    EXPECT_EQ(4, ExecuteInstruction(0x8F));
    EXPECT_EQ(1, get_instruction_ptr());
    EXPECT_REGISTER({{Register::A, 2}});
}

TEST_F(OpcodeHandlersTest, Adc8BitAB) {
    SetRegisterState({{Register::A, 100}, {Register::B, 200}, {Register::FC, 0}});
    EXPECT_EQ(0, get_instruction_ptr());
    EXPECT_EQ(4, ExecuteInstruction(0x88));
    EXPECT_EQ(1, get_instruction_ptr());
    EXPECT_REGISTER({{Register::A, 44}, {Register::B, 200}, {Register::FC, 1}});
}

TEST_F(OpcodeHandlersTest, Adc8BitAC) {
    SetRegisterState({{Register::A, 100}, {Register::C, 100}, {Register::FC, 1}});
    EXPECT_EQ(0, get_instruction_ptr());
    EXPECT_EQ(4, ExecuteInstruction(0x89));
    EXPECT_EQ(1, get_instruction_ptr());
    EXPECT_REGISTER({{Register::A, 201}, {Register::C, 100}, {Register::FC, 0}});
}

TEST_F(OpcodeHandlersTest, Adc8BitAD) {
    SetRegisterState({{Register::A, 100}, {Register::D, 100}, {Register::FC, 0}});
    EXPECT_EQ(0, get_instruction_ptr());
    EXPECT_EQ(4, ExecuteInstruction(0x8A));
    EXPECT_EQ(1, get_instruction_ptr());
    EXPECT_REGISTER({{Register::A, 200}, {Register::D, 100}, {Register::FC, 0}});
}

TEST_F(OpcodeHandlersTest, Adc8BitAE) {
    SetRegisterState({{Register::A, 100}, {Register::E, 200}, {Register::FC, 0}});
    EXPECT_EQ(0, get_instruction_ptr());
    EXPECT_EQ(4, ExecuteInstruction(0x8B));
    EXPECT_EQ(1, get_instruction_ptr());
    EXPECT_REGISTER({{Register::A, 44}, {Register::E, 200}, {Register::FC, 1}});
}

TEST_F(OpcodeHandlersTest, Adc8BitAH) {
    SetRegisterState({{Register::A, 100}, {Register::H, 200}, {Register::FC, 0}});
    EXPECT_EQ(0, get_instruction_ptr());
    EXPECT_EQ(4, ExecuteInstruction(0x8C));
    EXPECT_EQ(1, get_instruction_ptr());
    EXPECT_REGISTER({{Register::A, 44}, {Register::H, 200}, {Register::FC, 1}});
}

TEST_F(OpcodeHandlersTest, Adc8BitAL) {
    SetRegisterState({{Register::A, 100}, {Register::L, 200}, {Register::FC, 1}});
    EXPECT_EQ(0, get_instruction_ptr());
    EXPECT_EQ(4, ExecuteInstruction(0x8D));
    EXPECT_EQ(1, get_instruction_ptr());
    EXPECT_REGISTER({{Register::A, 45}, {Register::L, 200}, {Register::FC, 1}});
}

TEST_F(OpcodeHandlersTest, Adc8BitAHL) {
    SetRegisterState({{Register::A, 100}, {Register::HL, 200}, {Register::FC, 0}});
    EXPECT_EQ(0, get_instruction_ptr());
    EXPECT_EQ(8, ExecuteInstruction(0x8E));
    EXPECT_EQ(1, get_instruction_ptr());
    EXPECT_REGISTER({{Register::A, 44}, {Register::HL, 200}, {Register::FC, 1}});
}

TEST_F(OpcodeHandlersTest, Adc8BitALiteral) {
    SetRegisterState({{Register::A, 100}, {Register::FC, 1}});
    EXPECT_EQ(0, get_instruction_ptr());
    EXPECT_EQ(8, ExecuteInstruction(0xCE, static_cast<unsigned char>(100)));
    EXPECT_EQ(2, get_instruction_ptr());
    EXPECT_REGISTER({{Register::A, 201}, {Register::FC, 0}});
}

// Test SUB n
// Subtract n from A

TEST_F(OpcodeHandlersTest, Sub8BitA) {
    SetRegisterState({{Register::A, 2}});
    EXPECT_EQ(0, get_instruction_ptr());
    EXPECT_EQ(4, ExecuteInstruction(0x97));
    EXPECT_EQ(1, get_instruction_ptr());
    EXPECT_REGISTER({{Register::A, 0}});
}

TEST_F(OpcodeHandlersTest, Sub8BitB) {
    SetRegisterState({{Register::A, 2}, {Register::B, 1}});
    EXPECT_EQ(0, get_instruction_ptr());
    EXPECT_EQ(4, ExecuteInstruction(0x90));
    EXPECT_EQ(1, get_instruction_ptr());
    EXPECT_REGISTER({{Register::A, 1}, {Register::B, 1}});
}

TEST_F(OpcodeHandlersTest, Sub8BitC) {
    SetRegisterState({{Register::A, 2}, {Register::C, 1}});
    EXPECT_EQ(0, get_instruction_ptr());
    EXPECT_EQ(4, ExecuteInstruction(0x91));
    EXPECT_EQ(1, get_instruction_ptr());
    EXPECT_REGISTER({{Register::A, 1}, {Register::C, 1}});
}

TEST_F(OpcodeHandlersTest, Sub8BitD) {
    SetRegisterState({{Register::A, 2}, {Register::D, 1}});
    EXPECT_EQ(0, get_instruction_ptr());
    EXPECT_EQ(4, ExecuteInstruction(0x92));
    EXPECT_EQ(1, get_instruction_ptr());
    EXPECT_REGISTER({{Register::A, 1}, {Register::D, 1}});
}

TEST_F(OpcodeHandlersTest, Sub8BitE) {
    SetRegisterState({{Register::A, 2}, {Register::E, 1}});
    EXPECT_EQ(0, get_instruction_ptr());
    EXPECT_EQ(4, ExecuteInstruction(0x93));
    EXPECT_EQ(1, get_instruction_ptr());
    EXPECT_REGISTER({{Register::A, 1}, {Register::E, 1}});
}

TEST_F(OpcodeHandlersTest, Sub8BitH) {
    SetRegisterState({{Register::A, 2}, {Register::H, 1}});
    EXPECT_EQ(0, get_instruction_ptr());
    EXPECT_EQ(4, ExecuteInstruction(0x94));
    EXPECT_EQ(1, get_instruction_ptr());
    EXPECT_REGISTER({{Register::A, 1}, {Register::H, 1}});
}

TEST_F(OpcodeHandlersTest, Sub8BitL) {
    SetRegisterState({{Register::A, 2}, {Register::L, 1}});
    EXPECT_EQ(0, get_instruction_ptr());
    EXPECT_EQ(4, ExecuteInstruction(0x95));
    EXPECT_EQ(1, get_instruction_ptr());
    EXPECT_REGISTER({{Register::A, 1}, {Register::L, 1}});
}

TEST_F(OpcodeHandlersTest, Sub8BitHL) {
    SetRegisterState({{Register::A, 2}, {Register::HL, 1}});
    EXPECT_EQ(0, get_instruction_ptr());
    EXPECT_EQ(8, ExecuteInstruction(0x96));
    EXPECT_EQ(1, get_instruction_ptr());
    EXPECT_REGISTER({{Register::A, 1}, {Register::HL, 1}});
}

TEST_F(OpcodeHandlersTest, Sub8BitLiteral) {
    SetRegisterState({{Register::A, 2}});
    EXPECT_EQ(0, get_instruction_ptr());
    EXPECT_EQ(8, ExecuteInstruction(0xD6, static_cast<unsigned char>(1)));
    EXPECT_EQ(2, get_instruction_ptr());
    EXPECT_REGISTER({{Register::A, 1}});
}


// Test SBC A,n
// Subtract n + carry flag from A

TEST_F(OpcodeHandlersTest, Sbc8BitA) {
    SetRegisterState({{Register::A, 2}, {Register::FC, 1}});
    EXPECT_EQ(0, get_instruction_ptr());
    EXPECT_EQ(4, ExecuteInstruction(0x9F));
    EXPECT_EQ(1, get_instruction_ptr());
    EXPECT_REGISTER({{Register::A, 255}, {Register::FC, 0}});
}

TEST_F(OpcodeHandlersTest, Sbc8BitB) {
    SetRegisterState({{Register::A, 2}, {Register::B, 3}, {Register::FC, 1}});
    EXPECT_EQ(0, get_instruction_ptr());
    EXPECT_EQ(4, ExecuteInstruction(0x98));
    EXPECT_EQ(1, get_instruction_ptr());
    EXPECT_REGISTER({{Register::A, 254}, {Register::B, 3}, {Register::FC, 0}});
}

TEST_F(OpcodeHandlersTest, Sbc8BitC) {
    SetRegisterState({{Register::A, 2}, {Register::C, 1}, {Register::FC, 1}});
    EXPECT_EQ(0, get_instruction_ptr());
    EXPECT_EQ(4, ExecuteInstruction(0x99));
    EXPECT_EQ(1, get_instruction_ptr());
    EXPECT_REGISTER({{Register::A, 0}, {Register::C, 1}, {Register::FC, 1}});
}

TEST_F(OpcodeHandlersTest, Sbc8BitD) {
    SetRegisterState({{Register::A, 2}, {Register::D, 1}, {Register::FC, 1}});
    EXPECT_EQ(0, get_instruction_ptr());
    EXPECT_EQ(4, ExecuteInstruction(0x9A));
    EXPECT_EQ(1, get_instruction_ptr());
    EXPECT_REGISTER({{Register::A, 0}, {Register::D, 1}, {Register::FC, 1}});
}

TEST_F(OpcodeHandlersTest, Sbc8BitE) {
    SetRegisterState({{Register::A, 2}, {Register::E, 3}, {Register::FC, 1}});
    EXPECT_EQ(0, get_instruction_ptr());
    EXPECT_EQ(4, ExecuteInstruction(0x9B));
    EXPECT_EQ(1, get_instruction_ptr());
    EXPECT_REGISTER({{Register::A, 254}, {Register::E, 3}, {Register::FC, 0}});
}

TEST_F(OpcodeHandlersTest, Sbc8BitH) {
    SetRegisterState({{Register::A, 2}, {Register::H, 1}, {Register::FC, 1}});
    EXPECT_EQ(0, get_instruction_ptr());
    EXPECT_EQ(4, ExecuteInstruction(0x9C));
    EXPECT_EQ(1, get_instruction_ptr());
    EXPECT_REGISTER({{Register::A, 0}, {Register::H, 1}, {Register::FC, 1}});
}

TEST_F(OpcodeHandlersTest, Sbc8BitL) {
    SetRegisterState({{Register::A, 200}, {Register::L, 100}, {Register::FC, 1}});
    EXPECT_EQ(0, get_instruction_ptr());
    EXPECT_EQ(4, ExecuteInstruction(0x9D));
    EXPECT_EQ(1, get_instruction_ptr());
    EXPECT_REGISTER({{Register::A, 99}, {Register::L, 100}, {Register::FC, 1}});
}

TEST_F(OpcodeHandlersTest, Sbc8BitHL) {
    SetRegisterState({{Register::A, 2}, {Register::HL, 1}, {Register::FC, 1}});
    EXPECT_EQ(0, get_instruction_ptr());
    EXPECT_EQ(8, ExecuteInstruction(0x9E));
    EXPECT_EQ(1, get_instruction_ptr());
    EXPECT_REGISTER({{Register::A, 0}, {Register::HL, 1}, {Register::FC, 1}});
}

TEST_F(OpcodeHandlersTest, Sbc8BitLiteral) {
    SetRegisterState({{Register::A, 2}, {Register::FC, 1}});
    EXPECT_EQ(0, get_instruction_ptr());
    EXPECT_EQ(8, ExecuteInstruction(0xDE, static_cast<unsigned char>(5)));
    EXPECT_EQ(2, get_instruction_ptr());
    EXPECT_REGISTER({{Register::A, 252}, {Register::FC, 0}});
}

// Test AND n
// Logically AND n with A, result in A

TEST_F(OpcodeHandlersTest, And8BitA) {
    SetRegisterState({{Register::A, 1}});
    ExecuteInstruction(0xA7);
    EXPECT_REGISTER({{Register::A, 1}});
}

TEST_F(OpcodeHandlersTest, And8BitB) {
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

TEST_F(OpcodeHandlersTest, And8BitC) {
    SetRegisterState({{Register::A, 2}, {Register::C, 2}});
    EXPECT_EQ(4, ExecuteInstruction(0xA1));
    EXPECT_REGISTER({{Register::A, 2}, {Register::C, 2}});
}

TEST_F(OpcodeHandlersTest, And8BitD) {
    SetRegisterState({{Register::A, 1}, {Register::D, 2}});
    EXPECT_EQ(4, ExecuteInstruction(0xA2));
    EXPECT_REGISTER({{Register::A, 0}, {Register::D, 2}});
}

TEST_F(OpcodeHandlersTest, And8BitE) {
    SetRegisterState({{Register::A, 0}, {Register::E, 0}});
    EXPECT_EQ(4, ExecuteInstruction(0xA3));
    EXPECT_REGISTER({{Register::A, 0}, {Register::E, 0}});
}

TEST_F(OpcodeHandlersTest, And8BitH) {
    SetRegisterState({{Register::A, 2}, {Register::H, 9}});
    EXPECT_EQ(4, ExecuteInstruction(0xA4));
    EXPECT_REGISTER({{Register::A, 0}, {Register::H, 9}});
}

TEST_F(OpcodeHandlersTest, And8BitL) {
    SetRegisterState({{Register::A, 100}, {Register::L, 20}});
    EXPECT_EQ(4, ExecuteInstruction(0xA5));
    EXPECT_REGISTER({{Register::A, 4}, {Register::L, 20}});
}

TEST_F(OpcodeHandlersTest, And8BitHL) {
    SetRegisterState({{Register::A, 50}, {Register::HL, 3}});
    EXPECT_EQ(8, ExecuteInstruction(0xA6));
    EXPECT_REGISTER({{Register::A, 2}, {Register::HL, 3}});
}

TEST_F(OpcodeHandlersTest, And8BitLiteral) {
    SetRegisterState({{Register::A, 50}});
    EXPECT_EQ(8, ExecuteInstruction(0xE6, static_cast<unsigned char>(53)));
    EXPECT_REGISTER({{Register::A, 48}});
}

// Test OR n
// Logical OR n with A, result in A

TEST_F(OpcodeHandlersTest, Or8BitA) {
    SetRegisterState({{Register::A, 1}});
    EXPECT_EQ(4, ExecuteInstruction(0xB7));
    EXPECT_REGISTER({{Register::A, 1}});
}

TEST_F(OpcodeHandlersTest, Or8BitB) {
    SetRegisterState({{Register::A, 1}, {Register::B, 2}});
    EXPECT_EQ(4, ExecuteInstruction(0xB0));
    EXPECT_REGISTER({{Register::A, 3}, {Register::B, 2}});
}

TEST_F(OpcodeHandlersTest, Or8BitC) {
    SetRegisterState({{Register::A, 2}, {Register::C, 2}});
    EXPECT_EQ(4, ExecuteInstruction(0xB1));
    EXPECT_REGISTER({{Register::A, 2}, {Register::C, 2}});
}

TEST_F(OpcodeHandlersTest, Or8BitD) {
    SetRegisterState({{Register::A, 1}, {Register::D, 0}});
    EXPECT_EQ(4, ExecuteInstruction(0xB2));
    EXPECT_REGISTER({{Register::A, 1}, {Register::D, 0}});
}

TEST_F(OpcodeHandlersTest, Or8BitE) {
    SetRegisterState({{Register::A, 50}, {Register::E, 30}});
    EXPECT_EQ(4, ExecuteInstruction(0xB3));
    EXPECT_REGISTER({{Register::A, 62}, {Register::E, 30}});
}

TEST_F(OpcodeHandlersTest, Or8BitH) {
    SetRegisterState({{Register::A, 0}, {Register::H, 255}});
    EXPECT_EQ(4, ExecuteInstruction(0xB4));
    EXPECT_REGISTER({{Register::A, 255}, {Register::H, 255}});
}

TEST_F(OpcodeHandlersTest, Or8BitL) {
    SetRegisterState({{Register::A, 0}, {Register::L, 0}});
    EXPECT_EQ(4, ExecuteInstruction(0xB5));
    EXPECT_REGISTER({{Register::A, 0}, {Register::L, 0}});
}

TEST_F(OpcodeHandlersTest, Or8BitHL) {
    SetRegisterState({{Register::A, 29}, {Register::HL, 0}});
    EXPECT_EQ(8, ExecuteInstruction(0xB6));
    EXPECT_REGISTER({{Register::A, 29}, {Register::HL, 0}});
}

TEST_F(OpcodeHandlersTest, Or8BitLiteral) {
    SetRegisterState({{Register::A, 29}});
    EXPECT_EQ(8, ExecuteInstruction(0xF6, static_cast<unsigned char>(27)));
    EXPECT_REGISTER({{Register::A, 31}});
}

// Test XOR n
//  Logical exclusive OR n with register A, result in A.

TEST_F(OpcodeHandlersTest, Xor8BitA) {
    SetRegisterState({{Register::A, 10}});
    EXPECT_EQ(4, ExecuteInstruction(0xAF));
    EXPECT_REGISTER({{Register::A, 0}});
}

TEST_F(OpcodeHandlersTest, Xor8BitB) {
    SetRegisterState({{Register::A, 1}, {Register::B, 2}});
    ExecuteInstruction(0xA8);
    EXPECT_REGISTER({{Register::A, 3}, {Register::B, 2}});
}

TEST_F(OpcodeHandlersTest, Xor8BitC) {
    SetRegisterState({{Register::A, 2}, {Register::C, 2}});
    EXPECT_EQ(4, ExecuteInstruction(0xA9));
    EXPECT_REGISTER({{Register::A, 0}, {Register::C, 2}});
}

TEST_F(OpcodeHandlersTest, Xor8BitD) {
    SetRegisterState({{Register::A, 2}, {Register::D, 3}});
    EXPECT_EQ(4, ExecuteInstruction(0xAA));
    EXPECT_REGISTER({{Register::A, 1}, {Register::D, 3}});
}

TEST_F(OpcodeHandlersTest, Xor8BitE) {
    SetRegisterState({{Register::A, 1}, {Register::E, 0}});
    EXPECT_EQ(4, ExecuteInstruction(0xAB));
    EXPECT_REGISTER({{Register::A, 1}, {Register::E, 0}});
}

TEST_F(OpcodeHandlersTest, Xor8BitH) {
    SetRegisterState({{Register::A, 2}, {Register::H, 9}});
    EXPECT_EQ(4, ExecuteInstruction(0xAC));
    EXPECT_REGISTER({{Register::A, 11}, {Register::H, 9}});
}

TEST_F(OpcodeHandlersTest, Xor8BitL) {
    SetRegisterState({{Register::A, 20}, {Register::L, 10}});
    EXPECT_EQ(4, ExecuteInstruction(0xAD));
    EXPECT_REGISTER({{Register::A, 30}, {Register::L, 10}});
}

TEST_F(OpcodeHandlersTest, Xor8BitHL) {
    SetRegisterState({{Register::A, 2}, {Register::HL, 63}});
    EXPECT_EQ(8, ExecuteInstruction(0xAE));
    EXPECT_REGISTER({{Register::A, 61}, {Register::HL, 63}});
}

TEST_F(OpcodeHandlersTest, Xor8BitLiteral) {
    SetRegisterState({{Register::A, 2}});
    EXPECT_EQ(8, ExecuteInstruction(0xEE, static_cast<unsigned char>(10)));
    EXPECT_REGISTER({{Register::A, 8}});
}

// Tests for CP n
// Compare A with n. Basically does A - n subtraction with results thrown away

TEST_F(OpcodeHandlersTest, Cp8BitA) {
    SetRegisterState({{Register::A, 1}});
    EXPECT_EQ(4, ExecuteInstruction(0xBF));
    EXPECT_REGISTER({{Register::FZ, 1}, {Register::FN, 1}, {Register::FH, 0}, {Register::FC, 0}});
}

TEST_F(OpcodeHandlersTest, Cp8BitB) {
    SetRegisterState({{Register::A, 1}, {Register::B, 2}});
    EXPECT_EQ(4, ExecuteInstruction(0xB8));
    EXPECT_REGISTER({{Register::FZ, 0}, {Register::FN, 1}, {Register::FH, 1}, {Register::FC, 1}});
}

TEST_F(OpcodeHandlersTest, Cp8BitC) {
    SetRegisterState({{Register::A, 4}, {Register::C, 4}});
    EXPECT_EQ(4, ExecuteInstruction(0xB9));
    EXPECT_REGISTER({{Register::FZ, 1}, {Register::FN, 1}, {Register::FH, 0}, {Register::FC, 0}});
}

TEST_F(OpcodeHandlersTest, Cp8BitD) {
    SetRegisterState({{Register::A, 1}, {Register::D, 2}});
    EXPECT_EQ(4, ExecuteInstruction(0xBA));
    EXPECT_REGISTER({{Register::FZ, 0}, {Register::FN, 1}, {Register::FH, 1}, {Register::FC, 1}});
}

TEST_F(OpcodeHandlersTest, Cp8BitE) {
    SetRegisterState({{Register::A, 1}, {Register::E, 2}});
    EXPECT_EQ(4, ExecuteInstruction(0xBB));
    EXPECT_REGISTER({{Register::FZ, 0}, {Register::FN, 1}, {Register::FH, 1}, {Register::FC, 1}});
}

TEST_F(OpcodeHandlersTest, Cp8BitH) {
    SetRegisterState({{Register::A, 1}, {Register::H, 2}});
    EXPECT_EQ(4, ExecuteInstruction(0xBC));
    EXPECT_REGISTER({{Register::FZ, 0}, {Register::FN, 1}, {Register::FH, 1}, {Register::FC, 1}});
}

TEST_F(OpcodeHandlersTest, Cp8BitL) {
    SetRegisterState({{Register::A, 1}, {Register::L, 2}});
    EXPECT_EQ(4, ExecuteInstruction(0xBD));
    EXPECT_REGISTER({{Register::FZ, 0}, {Register::FN, 1}, {Register::FH, 1}, {Register::FC, 1}});
}

TEST_F(OpcodeHandlersTest, Cp8BitHL) {
    SetRegisterState({{Register::A, 1}, {Register::HL, 2}});
    EXPECT_EQ(8, ExecuteInstruction(0xBE));
    EXPECT_REGISTER({{Register::FZ, 0}, {Register::FN, 1}, {Register::FH, 1}, {Register::FC, 1}});
}

TEST_F(OpcodeHandlersTest, Cp8BitLiteral) {
    SetRegisterState({{Register::A, 1}});
    EXPECT_EQ(8, ExecuteInstruction(0xFE, static_cast<unsigned char>(2)));
    EXPECT_REGISTER({{Register::FZ, 0}, {Register::FN, 1}, {Register::FH, 1}, {Register::FC, 1}});
}






TEST_F(OpcodeHandlersTest, GetRegisterValue) {
    get_rom_ptr()[get_instruction_ptr()] = 0x80;
    SetRegisterState({{Register::B, 1}});

    int expected_register_value = 1;
    int actual_register_value = GetRegisterValue(get_rom_ptr(), get_instruction_ptr(), 0x80);
    EXPECT_EQ(expected_register_value, actual_register_value);
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

} // namespace handlers
} // namespace back_end
