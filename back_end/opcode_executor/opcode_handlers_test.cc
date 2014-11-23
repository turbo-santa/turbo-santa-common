#include <vector>
#include "back_end/opcode_executor/opcode_handlers.h"
#include "back_end/opcode_executor/opcode_executor.h"
#include "back_end/opcode_executor/opcodes.h"
#include "test_harness/test_harness.h"
#include "test_harness/test_harness_utils.h"

#include "third_party/gtest/include/gtest/gtest.h"

#include <glog/logging.h>

namespace back_end {
namespace handlers {
  
unsigned char GetRegisterValue(unsigned char* rom, int instruction_ptr, unsigned char opcode);

using std::vector;
using handlers::OpcodeExecutor;
using Register = test_harness::RegisterNameValuePair;
using Memory = test_harness::MemoryAddressValuePair;

// The fixture gets instantiated once per test case. We would like to reuse the
// OpcodeExecutor. Also, this will get cleaned up when the test is over.
OpcodeExecutor* parser = new OpcodeExecutor();

class OpcodeHandlersTest : public test_harness::TestHarness {
  protected:
    OpcodeHandlersTest() : test_harness::TestHarness(parser) {}
};

// Start 8-bit load tests

// LD r,n tests
// Put value n into r

TEST_F(OpcodeHandlersTest, LoadBLiteral) {
  SetRegisterState({{Register::B, 0}});
  ExecuteInstruction(static_cast<unsigned char>(0x06), static_cast<unsigned char>(0x1));
  EXPECT_REGISTER({{Register::B, 0x1}});
}

TEST_F(OpcodeHandlersTest, LoadCLiteral) {
  SetRegisterState({{Register::C, 0}});
  ExecuteInstruction(static_cast<unsigned char>(0x0E), static_cast<unsigned char>(0x1));
  EXPECT_REGISTER({{Register::C, 0x1}});
}

TEST_F(OpcodeHandlersTest, LoadDLiteral) {
  SetRegisterState({{Register::D, 0}});
  ExecuteInstruction(static_cast<unsigned char>(0x16), static_cast<unsigned char>(0x1));
  EXPECT_REGISTER({{Register::D, 0x1}});
}

TEST_F(OpcodeHandlersTest, LoadELiteral) {
  SetRegisterState({{Register::E, 0}});
  ExecuteInstruction(static_cast<unsigned char>(0x1E), static_cast<unsigned char>(0x1));
  EXPECT_REGISTER({{Register::E, 0x1}});
}

TEST_F(OpcodeHandlersTest, LoadHLiteral) {
  SetRegisterState({{Register::H, 0}});
  ExecuteInstruction(static_cast<unsigned char>(0x26), static_cast<unsigned char>(0x1));
  EXPECT_REGISTER({{Register::H, 0x1}});
}

TEST_F(OpcodeHandlersTest, LoadLLiteral) {
  SetRegisterState({{Register::L, 0}});
  ExecuteInstruction(static_cast<unsigned char>(0x2E), static_cast<unsigned char>(0x1));
  EXPECT_REGISTER({{Register::L, 0x1}});
}

// LD r1, r2 tests
// Put value r2 into r1

TEST_F(OpcodeHandlersTest, LoadAA) {
  SetRegisterState({{Register::A, 0xDE}});
  ExecuteInstruction(static_cast<unsigned char>(0x7F));
  EXPECT_REGISTER({{Register::A, 0xDE}});
}

TEST_F(OpcodeHandlersTest, LoadAB) {
  SetRegisterState({{Register::A, 0x01}, {Register::B, 0xDE}});
  ExecuteInstruction(static_cast<unsigned char>(0x78));
  EXPECT_REGISTER({{Register::A, 0xDE}, {Register::B, 0xDE}});
}

TEST_F(OpcodeHandlersTest, LoadAC) {
  SetRegisterState({{Register::A, 0x01}, {Register::C, 0xDE}});
  ExecuteInstruction(static_cast<unsigned char>(0x79));
  EXPECT_REGISTER({{Register::A, 0xDE}, {Register::C, 0xDE}});
}

TEST_F(OpcodeHandlersTest, LoadAD) {
  SetRegisterState({{Register::A, 0x01}, {Register::D, 0xDE}});
  ExecuteInstruction(static_cast<unsigned char>(0x7a));
  EXPECT_REGISTER({{Register::A, 0xDE}, {Register::D, 0xDE}});
}

TEST_F(OpcodeHandlersTest, LoadAE) {
  SetRegisterState({{Register::A, 0x01}, {Register::E, 0xDE}});
  ExecuteInstruction(static_cast<unsigned char>(0x7b));
  EXPECT_REGISTER({{Register::A, 0xDE}, {Register::E, 0xDE}});
}

TEST_F(OpcodeHandlersTest, LoadAH) {
  SetRegisterState({{Register::A, 0x01}, {Register::H, 0xDE}});
  ExecuteInstruction(static_cast<unsigned char>(0x7c));
  EXPECT_REGISTER({{Register::A, 0xDE}, {Register::H, 0xDE}});
}

TEST_F(OpcodeHandlersTest, LoadAL) {
  SetRegisterState({{Register::A, 0x01}, {Register::L, 0xDE}});
  ExecuteInstruction(static_cast<unsigned char>(0x7d));
  EXPECT_REGISTER({{Register::A, 0xDE}, {Register::L, 0xDE}});
}

TEST_F(OpcodeHandlersTest, LoadAHL) {
  SetRegisterState({{Register::A, 0x01}, {Register::HL, 0xC015}});
  SetMemoryState({{0xC015, 35}});
  ExecuteInstruction(static_cast<unsigned char>(0x7e));
  EXPECT_REGISTER({{Register::A, 35}, {Register::HL, 0xC015}});
}

TEST_F(OpcodeHandlersTest, LoadBB) {
  SetRegisterState({{Register::B, 0x01}, {Register::B, 0xDE}});
  ExecuteInstruction(static_cast<unsigned char>(0x40));
  EXPECT_REGISTER({{Register::B, 0xDE}, {Register::B, 0xDE}});
}

TEST_F(OpcodeHandlersTest, LoadBC) {
  SetRegisterState({{Register::B, 0x01}, {Register::C, 0x02}});
  ExecuteInstruction(static_cast<unsigned char>(0x41));
  EXPECT_REGISTER({{Register::B, 0x02}, {Register::C, 0x02}});
}

TEST_F(OpcodeHandlersTest, LoadBD) {
  SetRegisterState({{Register::B, 0x01}, {Register::D, 0xDE}});
  ExecuteInstruction(static_cast<unsigned char>(0x42));
  EXPECT_REGISTER({{Register::B, 0xDE}, {Register::D, 0xDE}});
}

TEST_F(OpcodeHandlersTest, LoadBE) {
  SetRegisterState({{Register::B, 0x01}, {Register::E, 0xDE}});
  ExecuteInstruction(static_cast<unsigned char>(0x43));
  EXPECT_REGISTER({{Register::B, 0xDE}, {Register::E, 0xDE}});
}

TEST_F(OpcodeHandlersTest, LoadBH) {
  SetRegisterState({{Register::B, 0x01}, {Register::H, 0xDE}});
  ExecuteInstruction(static_cast<unsigned char>(0x44));
  EXPECT_REGISTER({{Register::B, 0xDE}, {Register::H, 0xDE}});
}

TEST_F(OpcodeHandlersTest, LoadBL) {
  SetRegisterState({{Register::B, 0x01}, {Register::L, 0xDE}});
  EXPECT_EQ(4, ExecuteInstruction(static_cast<unsigned char>(0x45)));
  EXPECT_REGISTER({{Register::B, 0xDE}, {Register::L, 0xDE}});
}

TEST_F(OpcodeHandlersTest, LoadBHL) {
  SetRegisterState({{Register::B, 0x01}, {Register::HL, 0xC015}});
  SetMemoryState({{0xC015, 0xDE}});
  EXPECT_EQ(8, ExecuteInstruction(static_cast<unsigned char>(0x46)));
  EXPECT_REGISTER({{Register::B, 0xDE}, {Register::HL, 0xC015}});
  EXPECT_MEMORY({{0xC015, 0xDE}});
}

TEST_F(OpcodeHandlersTest, LoadCB) {
  SetRegisterState({{Register::C, 0x01}, {Register::B, 0xDE}});
  ExecuteInstruction(static_cast<unsigned char>(0x48));
  EXPECT_REGISTER({{Register::C, 0xDE}, {Register::B, 0xDE}});
}

TEST_F(OpcodeHandlersTest, LoadCC) {
  SetRegisterState({{Register::C, 0x01}, {Register::C, 0xDE}});
  ExecuteInstruction(static_cast<unsigned char>(0x49));
  EXPECT_REGISTER({{Register::C, 0xDE}, {Register::C, 0xDE}});
}

TEST_F(OpcodeHandlersTest, LoadCD) {
  SetRegisterState({{Register::C, 0x01}, {Register::D, 0xDE}});
  ExecuteInstruction(static_cast<unsigned char>(0x4A));
  EXPECT_REGISTER({{Register::C, 0xDE}, {Register::D, 0xDE}});
}

TEST_F(OpcodeHandlersTest, LoadCE) {
  SetRegisterState({{Register::C, 0x01}, {Register::E, 0xDE}});
  ExecuteInstruction(static_cast<unsigned char>(0x4B));
  EXPECT_REGISTER({{Register::C, 0xDE}, {Register::E, 0xDE}});
}

TEST_F(OpcodeHandlersTest, LoadCH) {
  SetRegisterState({{Register::C, 0x01}, {Register::H, 0xDE}});
  ExecuteInstruction(static_cast<unsigned char>(0x4C));
  EXPECT_REGISTER({{Register::C, 0xDE}, {Register::H, 0xDE}});
}

TEST_F(OpcodeHandlersTest, LoadCL) {
  SetRegisterState({{Register::C, 0x01}, {Register::L, 0xDE}});
  ExecuteInstruction(static_cast<unsigned char>(0x4D));
  EXPECT_REGISTER({{Register::C, 0xDE}, {Register::L, 0xDE}});
}

TEST_F(OpcodeHandlersTest, LoadCHL) {
  SetRegisterState({{Register::C, 0x01}, {Register::HL, 0xC015}});
  SetMemoryState({{0xC015, 0xDE}});
  ExecuteInstruction(static_cast<unsigned char>(0x4E));
  EXPECT_REGISTER({{Register::C, 0xDE}, {Register::HL, 0xC015}});
  EXPECT_MEMORY({{0xC015, 0xDE}});
}

TEST_F(OpcodeHandlersTest, LoadDB) {
  SetRegisterState({{Register::D, 0x01}, {Register::B, 0xDE}});
  ExecuteInstruction(static_cast<unsigned char>(0x50));
  EXPECT_REGISTER({{Register::D, 0xDE}, {Register::B, 0xDE}});
}

TEST_F(OpcodeHandlersTest, LoadDC) {
  SetRegisterState({{Register::D, 0x01}, {Register::C, 0xDE}});
  ExecuteInstruction(static_cast<unsigned char>(0x51));
  EXPECT_REGISTER({{Register::D, 0xDE}, {Register::C, 0xDE}});
}

TEST_F(OpcodeHandlersTest, LoadDD) {
  SetRegisterState({{Register::D, 0x01}, {Register::D, 0xDE}});
  ExecuteInstruction(static_cast<unsigned char>(0x52));
  EXPECT_REGISTER({{Register::D, 0xDE}, {Register::D, 0xDE}});
}

TEST_F(OpcodeHandlersTest, LoadDE) {
  SetRegisterState({{Register::D, 0x01}, {Register::E, 0xDE}});
  ExecuteInstruction(static_cast<unsigned char>(0x53));
  EXPECT_REGISTER({{Register::D, 0xDE}, {Register::E, 0xDE}});
}

TEST_F(OpcodeHandlersTest, LoadDH) {
  SetRegisterState({{Register::D, 0x01}, {Register::H, 0xDE}});
  ExecuteInstruction(static_cast<unsigned char>(0x54));
  EXPECT_REGISTER({{Register::D, 0xDE}, {Register::H, 0xDE}});
}

TEST_F(OpcodeHandlersTest, LoadDL) {
  SetRegisterState({{Register::D, 0x01}, {Register::L, 0xDE}});
  ExecuteInstruction(static_cast<unsigned char>(0x55));
  EXPECT_REGISTER({{Register::D, 0xDE}, {Register::L, 0xDE}});
}

TEST_F(OpcodeHandlersTest, LoadDHL) {
  SetRegisterState({{Register::D, 0x01}, {Register::HL, 0xC015}});
  SetMemoryState({{0xC015, 0xDE}});
  ExecuteInstruction(static_cast<unsigned char>(0x56));
  EXPECT_REGISTER({{Register::D, 0xDE}, {Register::HL, 0xC015}});
  EXPECT_MEMORY({{0xC015, 0xDE}});
}

TEST_F(OpcodeHandlersTest, LoadEB) {
  SetRegisterState({{Register::E, 0x01}, {Register::B, 0xDE}});
  ExecuteInstruction(static_cast<unsigned char>(0x58));
  EXPECT_REGISTER({{Register::E, 0xDE}, {Register::B, 0xDE}});
}

TEST_F(OpcodeHandlersTest, LoadEC) {
  SetRegisterState({{Register::E, 0x01}, {Register::C, 0xDE}});
  ExecuteInstruction(static_cast<unsigned char>(0x59));
  EXPECT_REGISTER({{Register::E, 0xDE}, {Register::C, 0xDE}});
}

TEST_F(OpcodeHandlersTest, LoadED) {
  SetRegisterState({{Register::E, 0x01}, {Register::D, 0xDE}});
  ExecuteInstruction(static_cast<unsigned char>(0x5A));
  EXPECT_REGISTER({{Register::E, 0xDE}, {Register::D, 0xDE}});
}

TEST_F(OpcodeHandlersTest, LoadEE) {
  SetRegisterState({{Register::E, 0x01}, {Register::E, 0xDE}});
  ExecuteInstruction(static_cast<unsigned char>(0x5B));
  EXPECT_REGISTER({{Register::E, 0xDE}, {Register::E, 0xDE}});
}

TEST_F(OpcodeHandlersTest, LoadEH) {
  SetRegisterState({{Register::E, 0x01}, {Register::H, 0xDE}});
  ExecuteInstruction(static_cast<unsigned char>(0x5C));
  EXPECT_REGISTER({{Register::E, 0xDE}, {Register::H, 0xDE}});
}

TEST_F(OpcodeHandlersTest, LoadEL) {
  SetRegisterState({{Register::E, 0x01}, {Register::L, 0xDE}});
  ExecuteInstruction(static_cast<unsigned char>(0x5D));
  EXPECT_REGISTER({{Register::E, 0xDE}, {Register::L, 0xDE}});
}

TEST_F(OpcodeHandlersTest, LoadEHL) {
  SetRegisterState({{Register::E, 0x01}, {Register::HL, 0xC015}});
  SetMemoryState({{0xC015, 0xDE}});
  ExecuteInstruction(static_cast<unsigned char>(0x5E));
  EXPECT_REGISTER({{Register::E, 0xDE}, {Register::HL, 0xC015}});
  EXPECT_MEMORY({{0xC015, 0xDE}});
}

TEST_F(OpcodeHandlersTest, LoadHB) {
  SetRegisterState({{Register::H, 0x01}, {Register::B, 0xDE}});
  ExecuteInstruction(static_cast<unsigned char>(0x60));
  EXPECT_REGISTER({{Register::H, 0xDE}, {Register::B, 0xDE}});
}

TEST_F(OpcodeHandlersTest, LoadHC) {
  SetRegisterState({{Register::H, 0x01}, {Register::C, 0xDE}});
  ExecuteInstruction(static_cast<unsigned char>(0x61));
  EXPECT_REGISTER({{Register::H, 0xDE}, {Register::C, 0xDE}});
}

TEST_F(OpcodeHandlersTest, LoadHD) {
  SetRegisterState({{Register::H, 0x01}, {Register::D, 0xDE}});
  ExecuteInstruction(static_cast<unsigned char>(0x62));
  EXPECT_REGISTER({{Register::H, 0xDE}, {Register::D, 0xDE}});
}

TEST_F(OpcodeHandlersTest, LoadHE) {
  SetRegisterState({{Register::H, 0x01}, {Register::E, 0xDE}});
  ExecuteInstruction(static_cast<unsigned char>(0x63));
  EXPECT_REGISTER({{Register::H, 0xDE}, {Register::E, 0xDE}});
}

TEST_F(OpcodeHandlersTest, LoadHH) {
  SetRegisterState({{Register::H, 0x01}, {Register::H, 0xDE}});
  ExecuteInstruction(static_cast<unsigned char>(0x64));
  EXPECT_REGISTER({{Register::H, 0xDE}, {Register::H, 0xDE}});
}

TEST_F(OpcodeHandlersTest, LoadHL) {
  SetRegisterState({{Register::H, 0x01}, {Register::L, 0xDE}});
  ExecuteInstruction(static_cast<unsigned char>(0x65));
  EXPECT_REGISTER({{Register::H, 0xDE}, {Register::L, 0xDE}});
}

TEST_F(OpcodeHandlersTest, LoadHHL) {
  SetRegisterState({{Register::H, 0xC0}, {Register::HL, 0xC015}});
  SetMemoryState({{0xC015, 0xDE}});
  ExecuteInstruction(static_cast<unsigned char>(0x66));
  EXPECT_REGISTER({{Register::H, 0xDE}, {Register::HL, 0xDE15}});
  EXPECT_MEMORY({{0xC015, 0xDE}});
}

TEST_F(OpcodeHandlersTest, LoadLB) {
  SetRegisterState({{Register::L, 0x01}, {Register::B, 0xDE}});
  ExecuteInstruction(static_cast<unsigned char>(0x68));
  EXPECT_REGISTER({{Register::L, 0xDE}, {Register::B, 0xDE}});
}

TEST_F(OpcodeHandlersTest, LoadLC) {
  SetRegisterState({{Register::L, 0x01}, {Register::C, 0xDE}});
  ExecuteInstruction(static_cast<unsigned char>(0x69));
  EXPECT_REGISTER({{Register::L, 0xDE}, {Register::C, 0xDE}});
}

TEST_F(OpcodeHandlersTest, LoadLD) {
  SetRegisterState({{Register::L, 0x01}, {Register::D, 0xDE}});
  ExecuteInstruction(static_cast<unsigned char>(0x6A));
  EXPECT_REGISTER({{Register::L, 0xDE}, {Register::D, 0xDE}});
}

TEST_F(OpcodeHandlersTest, LoadLE) {
  SetRegisterState({{Register::L, 0x01}, {Register::E, 0xDE}});
  ExecuteInstruction(static_cast<unsigned char>(0x6B));
  EXPECT_REGISTER({{Register::L, 0xDE}, {Register::E, 0xDE}});
}

TEST_F(OpcodeHandlersTest, LoadLH) {
  SetRegisterState({{Register::L, 0x01}, {Register::H, 0xDE}});
  ExecuteInstruction(static_cast<unsigned char>(0x6C));
  EXPECT_REGISTER({{Register::L, 0xDE}, {Register::H, 0xDE}});
}

TEST_F(OpcodeHandlersTest, LoadLL) {
  SetRegisterState({{Register::L, 0x01}, {Register::L, 0xDE}});
  ExecuteInstruction(static_cast<unsigned char>(0x6D));
  EXPECT_REGISTER({{Register::L, 0xDE}, {Register::L, 0xDE}});
}

TEST_F(OpcodeHandlersTest, LoadLHL) {
  SetRegisterState({{Register::L, 0x15}, {Register::HL, 0xC015}});
  SetMemoryState({{0xC015, 0xDE}});
  ExecuteInstruction(static_cast<unsigned char>(0x6E));
  EXPECT_REGISTER({{Register::L, 0xDE}, {Register::HL, 0xC0DE}});
  EXPECT_MEMORY({{0xC015, 0xDE}});
}

TEST_F(OpcodeHandlersTest, LoadHLB) {
  SetRegisterState({{Register::HL, 0xC015}, {Register::B, 0x3}});
  ExecuteInstruction(static_cast<unsigned char>(0x70));
  EXPECT_REGISTER({{Register::HL, 0xC015}, {Register::B, 0x3}});
  EXPECT_MEMORY({{0xC015, 0x3}});
}

TEST_F(OpcodeHandlersTest, LoadHLC) {
  SetRegisterState({{Register::HL, 0xC015}, {Register::C, 0x3}});
  ExecuteInstruction(static_cast<unsigned char>(0x71));
  EXPECT_REGISTER({{Register::HL, 0xC015}, {Register::C, 0x3}});
  EXPECT_MEMORY({{0xC015, 0x3}});
}

TEST_F(OpcodeHandlersTest, LoadHLD) {
  SetRegisterState({{Register::HL, 0xC015}, {Register::D, 0xF1}});
  ExecuteInstruction(static_cast<unsigned char>(0x72));
  EXPECT_REGISTER({{Register::HL, 0xC015}, {Register::D, 0xF1}});
  EXPECT_MEMORY({{0xC015, 0xF1}});
}

TEST_F(OpcodeHandlersTest, LoadHLE) {
  SetRegisterState({{Register::HL, 0xC015}, {Register::E, 0xFB}});
  ExecuteInstruction(static_cast<unsigned char>(0x73));
  EXPECT_REGISTER({{Register::HL, 0xC015}, {Register::E, 0xFB}});
  EXPECT_MEMORY({{0xC015, 0xFB}});
}

TEST_F(OpcodeHandlersTest, LoadHLH) {
  SetRegisterState({{Register::HL, 0xC015}, {Register::H, 0xC0}});
  ExecuteInstruction(static_cast<unsigned char>(0x74));
  EXPECT_REGISTER({{Register::HL, 0xC015}, {Register::H, 0xC0}});
  EXPECT_MEMORY({{0xC015, 0xC0}});
}

TEST_F(OpcodeHandlersTest, LoadHLL) {
  SetRegisterState({{Register::HL, 0xC015}, {Register::L, 0x15}});
  EXPECT_EQ(8, ExecuteInstruction(static_cast<unsigned char>(0x75)));
  EXPECT_REGISTER({{Register::HL, 0xC015}, {Register::L, 0x15}});
  EXPECT_MEMORY({{0xC015, 0x15}});
}
  
TEST_F(OpcodeHandlersTest, LoadHLN) {
  SetRegisterState({{Register::HL, 0xC015}});
  EXPECT_EQ(12, ExecuteInstruction(static_cast<unsigned char>(0x36), static_cast<unsigned char>(0x10)));
  EXPECT_REGISTER({{Register::HL, 0xC015}});
  EXPECT_MEMORY({{0xC015, 0x10}});
}

//LDD n,A
// Load A into n

TEST_F(OpcodeHandlersTest, LoadAA2) {
  SetRegisterState({{Register::A, 0xC015}, {Register::A, 0x015}});
  ExecuteInstruction(static_cast<unsigned char>(0x7F));
  EXPECT_REGISTER({{Register::A, 0x015}, {Register::A, 0x015}});
}

TEST_F(OpcodeHandlersTest, LoadBA) {
  SetRegisterState({{Register::A, 0xC015}, {Register::B, 0x015}});
  ExecuteInstruction(static_cast<unsigned char>(0x47));
  EXPECT_REGISTER({{Register::A, 0x015}, {Register::B, 0x015}});
}

TEST_F(OpcodeHandlersTest, LoadCA) {
  SetRegisterState({{Register::A, 0xC015}, {Register::C, 0x015}});
  ExecuteInstruction(static_cast<unsigned char>(0x4F));
  EXPECT_REGISTER({{Register::A, 0x015}, {Register::C, 0x015}});
}

TEST_F(OpcodeHandlersTest, LoadDA) {
  SetRegisterState({{Register::A, 0xC015}, {Register::D, 0x015}});
  ExecuteInstruction(static_cast<unsigned char>(0x57));
  EXPECT_REGISTER({{Register::A, 0x015}, {Register::D, 0x015}});
}

TEST_F(OpcodeHandlersTest, LoadEA) {
  SetRegisterState({{Register::A, 0xC015}, {Register::E, 0x015}});
  ExecuteInstruction(static_cast<unsigned char>(0x5F));
  EXPECT_REGISTER({{Register::A, 0x015}, {Register::E, 0x015}});
}

TEST_F(OpcodeHandlersTest, LoadHA) {
  SetRegisterState({{Register::A, 0xC015}, {Register::H, 0x015}});
  ExecuteInstruction(static_cast<unsigned char>(0x67));
  EXPECT_REGISTER({{Register::A, 0x015}, {Register::H, 0x015}});
}

TEST_F(OpcodeHandlersTest, LoadLA) {
  SetRegisterState({{Register::A, 0xC015}, {Register::L, 0x015}});
  ExecuteInstruction(static_cast<unsigned char>(0x6F));
  EXPECT_REGISTER({{Register::A, 0x015}, {Register::L, 0x015}});
}

TEST_F(OpcodeHandlersTest, LoadBCA) {
  SetRegisterState({{Register::A, 0x01}, {Register::BC, 0xC011}});
  ExecuteInstruction(static_cast<unsigned char>(0x02));
  EXPECT_REGISTER({{Register::A, 0x01}, {Register::BC, 0xC011}});
  EXPECT_MEMORY({{0xC011, 0x01}})
}

TEST_F(OpcodeHandlersTest, LoadDEA) {
  SetRegisterState({{Register::A, 0x01}, {Register::DE, 0xC011}});
  ExecuteInstruction(static_cast<unsigned char>(0x12));
  EXPECT_REGISTER({{Register::A, 0x01}, {Register::DE, 0xC011}});
  EXPECT_MEMORY({{0xC011, 0x01}})
}

TEST_F(OpcodeHandlersTest, LoadHLA) {
  SetRegisterState({{Register::A, 0x01}, {Register::HL, 0xC011}});
  ExecuteInstruction(static_cast<unsigned char>(0x77));
  EXPECT_REGISTER({{Register::A, 0x01}, {Register::HL, 0xC011}});
  EXPECT_MEMORY({{0xC011, 0x01}})
}

// LD A,n
// Load value at n into A

TEST_F(OpcodeHandlersTest, LoadABC) {
  SetRegisterState({{Register::A, 0x30}, {Register::BC, 0xC011}});
  SetMemoryState({{0xC011, 0x10}});
  EXPECT_EQ(8, ExecuteInstruction(static_cast<unsigned char>(0x0A)));
  EXPECT_REGISTER({{Register::A, 0x10}, {Register::BC, 0xC011}});
  EXPECT_MEMORY({{0xC011, 0x10}});
}

TEST_F(OpcodeHandlersTest, LoadADE) {
  SetRegisterState({{Register::A, 0x30}, {Register::DE, 0xC011}});
  SetMemoryState({{0xC011, 0x10}});
  EXPECT_EQ(8, ExecuteInstruction(static_cast<unsigned char>(0x1A)));
  EXPECT_REGISTER({{Register::A, 0x10}, {Register::DE, 0xC011}});
  EXPECT_MEMORY({{0xC011, 0x10}});
}

// LD A, (C)
// Put value at address $FF00 + register C into A

TEST_F(OpcodeHandlersTest, LDAMemC) {
  SetRegisterState({{Register::A, 0x02}, {Register::C, 0x0F}});
  SetMemoryState({{0xFF0F, 0x05}});
  EXPECT_EQ(8, ExecuteInstruction(static_cast<unsigned char>(0xF2)));
  EXPECT_REGISTER({{Register::A, 0x05}, {Register::C, 0x0F}});
  EXPECT_MEMORY({{0xFF0F, 0x05}});
}

TEST_F(OpcodeHandlersTest, LDMemCA) {
  SetRegisterState({{Register::A, 0x02}, {Register::C, 0x0F}});
  SetMemoryState({{0xFF0F, 0x10}});
  EXPECT_EQ(8, ExecuteInstruction(static_cast<unsigned char>(0xE2)));
  EXPECT_REGISTER({{Register::A, 0x02}, {Register::C, 0x0F}});
  EXPECT_MEMORY({{0xFF0F, 0x02}});
}
 
// LDD tests

TEST_F(OpcodeHandlersTest, LoadDecAHL) {
  SetRegisterState({{Register::A, 0x03}, {Register::HL, 0xC015}});
  SetMemoryState({{0xC015, 0x10}});
  EXPECT_EQ(8, ExecuteInstruction(static_cast<unsigned char>(0x3A)));
  EXPECT_REGISTER({{Register::A, 0x10}, {Register::HL, 0xC014}});
  EXPECT_MEMORY({{0xC015, 0x10}});
}

TEST_F(OpcodeHandlersTest, LoadDecHLA) {
  SetRegisterState({{Register::A, 0x03}, {Register::HL, 0xC015}});
  SetMemoryState({{0xC015, 0x10}});
  EXPECT_EQ(8, ExecuteInstruction(static_cast<unsigned char>(0x32)));
  EXPECT_REGISTER({{Register::A, 0x03}, {Register::HL, 0xC014}});
  EXPECT_MEMORY({{0xC015, 0x03}});
}

TEST_F(OpcodeHandlersTest, LoadIncAHL) {
  SetRegisterState({{Register::A, 0x03}, {Register::HL, 0xC015}});
  SetMemoryState({{0xC015, 0x10}});
  EXPECT_EQ(8, ExecuteInstruction(static_cast<unsigned char>(0x2A)));
  EXPECT_REGISTER({{Register::A, 0x10}, {Register::HL, 0xC016}});
  EXPECT_MEMORY({{0xC015, 0x10}});
}

TEST_F(OpcodeHandlersTest, LoadIncHLA) {
  SetRegisterState({{Register::A, 0x03}, {Register::HL, 0xC015}});
  SetMemoryState({{0xC015, 0x10}});
  EXPECT_EQ(8, ExecuteInstruction(static_cast<unsigned char>(0x22)));
  EXPECT_REGISTER({{Register::A, 0x03}, {Register::HL, 0xC016}});
  EXPECT_MEMORY({{0xC015, 0x03}});
}

TEST_F(OpcodeHandlersTest, LDHnA) {
  SetRegisterState({{Register::A, 0x10}});
  EXPECT_EQ(12, ExecuteInstruction(static_cast<unsigned char>(0xE0), static_cast<unsigned char>(0x0F)));
  EXPECT_REGISTER({{Register::A, 0x10}});
  EXPECT_MEMORY({{0xFF0F, 0x10}});
}

TEST_F(OpcodeHandlersTest, LDHAn) {
  SetRegisterState({{Register::A, 0x10}});
  SetMemoryState({{0xFF0F, 0x05}});
  EXPECT_EQ(12, ExecuteInstruction(static_cast<unsigned char>(0xF0), static_cast<unsigned char>(0x0F)));
  EXPECT_REGISTER({{Register::A, 0x05}});
  EXPECT_MEMORY({{0xFF0F, 0x05}});
}

// End 8-bit load tests

// Begin 16-bit load tests
  
TEST_F(OpcodeHandlersTest, LoadBCNN) {
  SetRegisterState({{Register::BC, 0x1040}});
  EXPECT_EQ(12, ExecuteInstruction(static_cast<unsigned char>(0x01), static_cast<unsigned short>(0x3412)));
  EXPECT_REGISTER({{Register::BC, 0x1234}});
}

TEST_F(OpcodeHandlersTest, LoadDENN) {
  SetRegisterState({{Register::DE, 0x1040}});
  EXPECT_EQ(12, ExecuteInstruction(static_cast<unsigned char>(0x11), static_cast<unsigned short>(0x3412)));
  EXPECT_REGISTER({{Register::DE, 0x1234}});
}

TEST_F(OpcodeHandlersTest, LoadHLNN) {
  SetRegisterState({{Register::HL, 0x1040}});
  EXPECT_EQ(12, ExecuteInstruction(static_cast<unsigned char>(0x21), static_cast<unsigned short>(0x3412)));
  EXPECT_REGISTER({{Register::HL, 0x1234}});
}

TEST_F(OpcodeHandlersTest, LoadSPNN) {
  SetRegisterState({{Register::SP, 0x1040}});
  EXPECT_EQ(12, ExecuteInstruction(static_cast<unsigned char>(0x31), static_cast<unsigned short>(0x3412)));
  EXPECT_REGISTER({{Register::SP, 0x1234}});
}
  
TEST_F(OpcodeHandlersTest, LoadSPHL) {
  SetRegisterState({{Register::SP, 0x3344}, {Register::HL, 0x1234}});
  EXPECT_EQ(8, ExecuteInstruction(static_cast<unsigned char>(0xF9)));
  EXPECT_REGISTER({{Register::SP, 0x1234}, {Register::HL, 0x1234}});
}
  
TEST_F(OpcodeHandlersTest, LoadSPN) {
  SetRegisterState({{Register::SP, 0x334C}, {Register::HL, 0x1234}});
  EXPECT_EQ(12, ExecuteInstruction(static_cast<unsigned char>(0xF8), static_cast<unsigned char>(0x48)));
  EXPECT_REGISTER({{Register::SP, 0x334C}, {Register::HL, 0x3394}, {Register::FZ, 0}, {Register::FN, 0}, {Register::FH, 1}, {Register::FC, 0}});
}
  
TEST_F(OpcodeHandlersTest, LoadNNSP) {
  SetRegisterState({{Register::SP, 0x2030}});
  EXPECT_EQ(20, ExecuteInstruction(static_cast<unsigned char>(0x08), static_cast<unsigned short>(0x22C0)));
  EXPECT_MEMORY({{0xC022, 0x30}, {0xC023, 0x20}});
}
  
TEST_F(OpcodeHandlersTest, PushAF) {
  SetRegisterState({{Register::AF, 0x1234}, {Register::SP, 0xE000}});
  EXPECT_EQ(16, ExecuteInstruction(static_cast<unsigned char>(0xF5)));
  EXPECT_MEMORY({{0xDFFF, 0x34}, {0xDFFE, 0x12}});
  EXPECT_REGISTER({{Register::SP, 0xDFFE}, {Register::AF, 0x1234}});
}
  
TEST_F(OpcodeHandlersTest, PushBC) {
  SetRegisterState({{Register::BC, 0x1234}, {Register::SP, 0xE000}});
  EXPECT_EQ(16, ExecuteInstruction(static_cast<unsigned char>(0xC5)));
  EXPECT_MEMORY({{0xDFFF, 0x34}, {0xDFFE, 0x12}});
  EXPECT_REGISTER({{Register::SP, 0xDFFE}, {Register::BC, 0x1234}});
}

TEST_F(OpcodeHandlersTest, PushDE) {
  SetRegisterState({{Register::DE, 0x1234}, {Register::SP, 0xE000}});
  EXPECT_EQ(16, ExecuteInstruction(static_cast<unsigned char>(0xD5)));
  EXPECT_MEMORY({{0xDFFF, 0x34}, {0xDFFE, 0x12}});
  EXPECT_REGISTER({{Register::SP, 0xDFFE}, {Register::DE, 0x1234}});
}

TEST_F(OpcodeHandlersTest, PushHL) {
  SetRegisterState({{Register::HL, 0x1234}, {Register::SP, 0xE000}});
  EXPECT_EQ(16, ExecuteInstruction(static_cast<unsigned char>(0xE5)));
  EXPECT_MEMORY({{0xDFFF, 0x34}, {0xDFFE, 0x12}});
  EXPECT_REGISTER({{Register::SP, 0xDFFE}, {Register::HL, 0x1234}});
}
  
TEST_F(OpcodeHandlersTest, PopAF) {
  SetRegisterState({{Register::AF, 0x1111}, {Register::SP, 0xDFEE}});
  SetMemoryState({{0xDFEF, 0x30}, {0xDFF0, 0x55}});
  EXPECT_EQ(12, ExecuteInstruction(static_cast<unsigned char>(0xF1)));
  EXPECT_MEMORY({{0xDFEF, 0x30}, {0xDFF0, 0x55}});
  EXPECT_REGISTER({{Register::AF, 0x5530}, {Register::SP, 0xDFF0}});
}

TEST_F(OpcodeHandlersTest, PopBC) {
  SetRegisterState({{Register::BC, 0x1111}, {Register::SP, 0xDFEE}});
  SetMemoryState({{0xDFEF, 0x30}, {0xDFF0, 0x55}});
  EXPECT_EQ(12, ExecuteInstruction(static_cast<unsigned char>(0xC1)));
  EXPECT_MEMORY({{0xDFEF, 0x30}, {0xDFF0, 0x55}});
  EXPECT_REGISTER({{Register::BC, 0x5530}, {Register::SP, 0xDFF0}});
}

TEST_F(OpcodeHandlersTest, PopDE) {
  SetRegisterState({{Register::DE, 0x1111}, {Register::SP, 0xDFEE}});
  SetMemoryState({{0xDFEF, 0x30}, {0xDFF0, 0x55}});
  EXPECT_EQ(12, ExecuteInstruction(static_cast<unsigned char>(0xD1)));
  EXPECT_MEMORY({{0xDFEF, 0x30}, {0xDFF0, 0x55}});
  EXPECT_REGISTER({{Register::DE, 0x5530}, {Register::SP, 0xDFF0}});
}

TEST_F(OpcodeHandlersTest, PopHL) {
  SetRegisterState({{Register::HL, 0x1111}, {Register::SP, 0xDFEE}});
  SetMemoryState({{0xDFEF, 0x30}, {0xDFF0, 0x55}});
  EXPECT_EQ(12, ExecuteInstruction(static_cast<unsigned char>(0xE1)));
  EXPECT_MEMORY({{0xDFEF, 0x30}, {0xDFF0, 0x55}});
  EXPECT_REGISTER({{Register::HL, 0x5530}, {Register::SP, 0xDFF0}});
}
  
// End 16-bit load tests

// Begin tests for 8-bit ALU

// Tests for ADD A,n

TEST_F(OpcodeHandlersTest, Add8BitAA) {
  SetRegisterState({{Register::A, 0x1}});
  EXPECT_EQ(0, instruction_ptr());
  EXPECT_EQ(4, ExecuteInstruction(static_cast<unsigned char>(0x87)));
  EXPECT_EQ(1, instruction_ptr());
  EXPECT_REGISTER({{Register::A, 0x2}});
}

TEST_F(OpcodeHandlersTest, Add8BitAB) {
  SetRegisterState({{Register::A, 1}, {Register::B, 2}});
  EXPECT_EQ(0, instruction_ptr());
  EXPECT_EQ(4, ExecuteInstruction(static_cast<unsigned char>(0x80)));
  EXPECT_EQ(1, instruction_ptr());
  EXPECT_REGISTER({{Register::A, 3}, {Register::B, 2}});
}

TEST_F(OpcodeHandlersTest, Add8BitAC) {
  SetRegisterState({{Register::A, 0xFF}, {Register::C, 0x1}, {Register::FC, 0}});
  EXPECT_EQ(0, instruction_ptr());
  EXPECT_EQ(4, ExecuteInstruction(static_cast<unsigned char>(0x81)));
  EXPECT_EQ(1, instruction_ptr());
  EXPECT_REGISTER({{Register::A, 0x0}, {Register::C, 0x1}, {Register::FC, 1}});
}

TEST_F(OpcodeHandlersTest, Add8BitAD) {
  SetRegisterState({{Register::A, 1}, {Register::D, 2}});
  EXPECT_EQ(0, instruction_ptr());
  EXPECT_EQ(4, ExecuteInstruction(static_cast<unsigned char>(0x82)));
  EXPECT_EQ(1, instruction_ptr());
  EXPECT_REGISTER({{Register::A, 3}, {Register::D, 2}});
}

TEST_F(OpcodeHandlersTest, Add8BitAE) {
  SetRegisterState({{Register::A, 1}, {Register::E, 2}});
  EXPECT_EQ(0, instruction_ptr());
  EXPECT_EQ(4, ExecuteInstruction(static_cast<unsigned char>(0x83)));
  EXPECT_EQ(1, instruction_ptr());
  EXPECT_REGISTER({{Register::A, 3}, {Register::E, 2}});
}

TEST_F(OpcodeHandlersTest, Add8BitAH) {
  SetRegisterState({{Register::A, 1}, {Register::H, 2}});
  EXPECT_EQ(0, instruction_ptr());
  EXPECT_EQ(4, ExecuteInstruction(static_cast<unsigned char>(0x84)));
  EXPECT_EQ(1, instruction_ptr());
  EXPECT_REGISTER({{Register::A, 3}, {Register::H, 2}});
}

TEST_F(OpcodeHandlersTest, Add8BitAL) {
  SetRegisterState({{Register::A, 1}, {Register::L, 2}});
  EXPECT_EQ(0, instruction_ptr());
  EXPECT_EQ(4, ExecuteInstruction(static_cast<unsigned char>(0x85)));
  EXPECT_EQ(1, instruction_ptr());
  EXPECT_REGISTER({{Register::A, 3}, {Register::L, 2}});
}

TEST_F(OpcodeHandlersTest, Add8BitAHL) {
  SetMemoryState({{0xC015, 2}});
  SetRegisterState({{Register::A, 1}, {Register::HL, 0xC015}});
  EXPECT_EQ(0, instruction_ptr());
  EXPECT_EQ(8, ExecuteInstruction(static_cast<unsigned char>(0x86)));
  EXPECT_EQ(1, instruction_ptr());
  EXPECT_REGISTER({{Register::A, 3}, {Register::HL, 0xC015}});
}

TEST_F(OpcodeHandlersTest, Add8BitALiteral) {
  SetRegisterState({{Register::A, 1}});
  EXPECT_EQ(0, instruction_ptr());
  EXPECT_EQ(8, ExecuteInstruction(static_cast<unsigned char>(0xC6), static_cast<unsigned char>(5)));
  EXPECT_EQ(2, instruction_ptr());
  EXPECT_REGISTER({{Register::A, 6}});
}

// Tests for ADC A,n
// Add n + Carry flag to A

TEST_F(OpcodeHandlersTest, Adc8BitAA) {
  SetRegisterState({{Register::A, 1}});
  EXPECT_EQ(0, instruction_ptr());
  EXPECT_EQ(4, ExecuteInstruction(static_cast<unsigned char>(0x8F)));
  EXPECT_EQ(1, instruction_ptr());
  EXPECT_REGISTER({{Register::A, 2}});
}

TEST_F(OpcodeHandlersTest, Adc8BitAB) {
  SetRegisterState({{Register::A, 100}, {Register::B, 200}, {Register::FC, 0}});
  EXPECT_EQ(0, instruction_ptr());
  EXPECT_EQ(4, ExecuteInstruction(static_cast<unsigned char>(0x88)));
  EXPECT_EQ(1, instruction_ptr());
  EXPECT_REGISTER({{Register::A, 44}, {Register::B, 200}, {Register::FC, 1}});
}

TEST_F(OpcodeHandlersTest, Adc8BitAC) {
  SetRegisterState({{Register::A, 100}, {Register::C, 100}, {Register::FC, 1}});
  EXPECT_EQ(0, instruction_ptr());
  EXPECT_EQ(4, ExecuteInstruction(static_cast<unsigned char>(0x89)));
  EXPECT_EQ(1, instruction_ptr());
  EXPECT_REGISTER({{Register::A, 201}, {Register::C, 100}, {Register::FC, 0}});
}

TEST_F(OpcodeHandlersTest, Adc8BitAD) {
  SetRegisterState({{Register::A, 100}, {Register::D, 100}, {Register::FC, 0}});
  EXPECT_EQ(0, instruction_ptr());
  EXPECT_EQ(4, ExecuteInstruction(static_cast<unsigned char>(0x8A)));
  EXPECT_EQ(1, instruction_ptr());
  EXPECT_REGISTER({{Register::A, 200}, {Register::D, 100}, {Register::FC, 0}});
}

TEST_F(OpcodeHandlersTest, Adc8BitAE) {
  SetRegisterState({{Register::A, 100}, {Register::E, 200}, {Register::FC, 0}});
  EXPECT_EQ(0, instruction_ptr());
  EXPECT_EQ(4, ExecuteInstruction(static_cast<unsigned char>(0x8B)));
  EXPECT_EQ(1, instruction_ptr());
  EXPECT_REGISTER({{Register::A, 44}, {Register::E, 200}, {Register::FC, 1}});
}

TEST_F(OpcodeHandlersTest, Adc8BitAH) {
  SetRegisterState({{Register::A, 100}, {Register::H, 200}, {Register::FC, 0}});
  EXPECT_EQ(0, instruction_ptr());
  EXPECT_EQ(4, ExecuteInstruction(static_cast<unsigned char>(0x8C)));
  EXPECT_EQ(1, instruction_ptr());
  EXPECT_REGISTER({{Register::A, 44}, {Register::H, 200}, {Register::FC, 1}});
}

TEST_F(OpcodeHandlersTest, Adc8BitAL) {
  SetRegisterState({{Register::A, 100}, {Register::L, 200}, {Register::FC, 1}});
  EXPECT_EQ(0, instruction_ptr());
  EXPECT_EQ(4, ExecuteInstruction(static_cast<unsigned char>(0x8D)));
  EXPECT_EQ(1, instruction_ptr());
  EXPECT_REGISTER({{Register::A, 45}, {Register::L, 200}, {Register::FC, 1}});
}

TEST_F(OpcodeHandlersTest, Adc8BitAHL) {
  SetRegisterState({{Register::A, 100}, {Register::HL, 0xC015}, {Register::FC, 0}});
  SetMemoryState({{0xC015, 200}});
  EXPECT_EQ(0, instruction_ptr());
  EXPECT_EQ(8, ExecuteInstruction(static_cast<unsigned char>(0x8E)));
  EXPECT_EQ(1, instruction_ptr());
  EXPECT_REGISTER({{Register::A, 44}, {Register::HL, 0xC015}, {Register::FC, 1}});
}

TEST_F(OpcodeHandlersTest, Adc8BitALiteral) {
  SetRegisterState({{Register::A, 100}, {Register::FC, 1}});
  EXPECT_EQ(0, instruction_ptr());
  EXPECT_EQ(8, ExecuteInstruction(static_cast<unsigned char>(0xCE), static_cast<unsigned char>(100)));
  EXPECT_EQ(2, instruction_ptr());
  EXPECT_REGISTER({{Register::A, 201}, {Register::FC, 0}});
}

// Test SUB n
// Subtract n from A

TEST_F(OpcodeHandlersTest, Sub8BitA) {
  SetRegisterState({{Register::A, 2}});
  EXPECT_EQ(0, instruction_ptr());
  EXPECT_EQ(4, ExecuteInstruction(static_cast<unsigned char>(0x97)));
  EXPECT_EQ(1, instruction_ptr());
  EXPECT_REGISTER({{Register::A, 0}});
}

TEST_F(OpcodeHandlersTest, Sub8BitB) {
  SetRegisterState({{Register::A, 2}, {Register::B, 1}});
  EXPECT_EQ(0, instruction_ptr());
  EXPECT_EQ(4, ExecuteInstruction(static_cast<unsigned char>(0x90)));
  EXPECT_EQ(1, instruction_ptr());
  EXPECT_REGISTER({{Register::A, 1}, {Register::B, 1}});
}

TEST_F(OpcodeHandlersTest, Sub8BitC) {
  SetRegisterState({{Register::A, 2}, {Register::C, 1}});
  EXPECT_EQ(0, instruction_ptr());
  EXPECT_EQ(4, ExecuteInstruction(static_cast<unsigned char>(0x91)));
  EXPECT_EQ(1, instruction_ptr());
  EXPECT_REGISTER({{Register::A, 1}, {Register::C, 1}});
}

TEST_F(OpcodeHandlersTest, Sub8BitD) {
  SetRegisterState({{Register::A, 2}, {Register::D, 1}});
  EXPECT_EQ(0, instruction_ptr());
  EXPECT_EQ(4, ExecuteInstruction(static_cast<unsigned char>(0x92)));
  EXPECT_EQ(1, instruction_ptr());
  EXPECT_REGISTER({{Register::A, 1}, {Register::D, 1}});
}

TEST_F(OpcodeHandlersTest, Sub8BitE) {
  SetRegisterState({{Register::A, 2}, {Register::E, 1}});
  EXPECT_EQ(0, instruction_ptr());
  EXPECT_EQ(4, ExecuteInstruction(static_cast<unsigned char>(0x93)));
  EXPECT_EQ(1, instruction_ptr());
  EXPECT_REGISTER({{Register::A, 1}, {Register::E, 1}});
}

TEST_F(OpcodeHandlersTest, Sub8BitH) {
  SetRegisterState({{Register::A, 2}, {Register::H, 1}});
  EXPECT_EQ(0, instruction_ptr());
  EXPECT_EQ(4, ExecuteInstruction(static_cast<unsigned char>(0x94)));
  EXPECT_EQ(1, instruction_ptr());
  EXPECT_REGISTER({{Register::A, 1}, {Register::H, 1}});
}

TEST_F(OpcodeHandlersTest, Sub8BitL) {
  SetRegisterState({{Register::A, 2}, {Register::L, 1}});
  EXPECT_EQ(0, instruction_ptr());
  EXPECT_EQ(4, ExecuteInstruction(static_cast<unsigned char>(0x95)));
  EXPECT_EQ(1, instruction_ptr());
  EXPECT_REGISTER({{Register::A, 1}, {Register::L, 1}});
}

TEST_F(OpcodeHandlersTest, Sub8BitHL) {
  SetRegisterState({{Register::A, 2}, {Register::HL, 0xC015}});
  SetMemoryState({{0xC015, 1}});
  EXPECT_EQ(0, instruction_ptr());
  EXPECT_EQ(8, ExecuteInstruction(static_cast<unsigned char>(0x96)));
  EXPECT_EQ(1, instruction_ptr());
  EXPECT_REGISTER({{Register::A, 1}, {Register::HL, 0xC015}});
}

TEST_F(OpcodeHandlersTest, Sub8BitLiteral) {
  SetRegisterState({{Register::A, 2}});
  EXPECT_EQ(0, instruction_ptr());
  EXPECT_EQ(8, ExecuteInstruction(static_cast<unsigned char>(0xD6), static_cast<unsigned char>(1)));
  EXPECT_EQ(2, instruction_ptr());
  EXPECT_REGISTER({{Register::A, 1}});
}


// Test SBC A,n
// Subtract n + carry flag from A

TEST_F(OpcodeHandlersTest, Sbc8BitA) {
  SetRegisterState({{Register::A, 2}, {Register::FC, 1}});
  EXPECT_EQ(0, instruction_ptr());
  EXPECT_EQ(4, ExecuteInstruction(static_cast<unsigned char>(0x9F)));
  EXPECT_EQ(1, instruction_ptr());
  EXPECT_REGISTER({{Register::A, 255}, {Register::FC, 0}});
}

TEST_F(OpcodeHandlersTest, Sbc8BitB) {
  SetRegisterState({{Register::A, 2}, {Register::B, 3}, {Register::FC, 1}});
  EXPECT_EQ(0, instruction_ptr());
  EXPECT_EQ(4, ExecuteInstruction(static_cast<unsigned char>(0x98)));
  EXPECT_EQ(1, instruction_ptr());
  EXPECT_REGISTER({{Register::A, 254}, {Register::B, 3}, {Register::FC, 0}});
}

TEST_F(OpcodeHandlersTest, Sbc8BitC) {
  SetRegisterState({{Register::A, 2}, {Register::C, 1}, {Register::FC, 1}});
  EXPECT_EQ(0, instruction_ptr());
  EXPECT_EQ(4, ExecuteInstruction(static_cast<unsigned char>(0x99)));
  EXPECT_EQ(1, instruction_ptr());
  EXPECT_REGISTER({{Register::A, 0}, {Register::C, 1}, {Register::FC, 1}});
}

TEST_F(OpcodeHandlersTest, Sbc8BitD) {
  SetRegisterState({{Register::A, 2}, {Register::D, 1}, {Register::FC, 1}});
  EXPECT_EQ(0, instruction_ptr());
  EXPECT_EQ(4, ExecuteInstruction(static_cast<unsigned char>(0x9A)));
  EXPECT_EQ(1, instruction_ptr());
  EXPECT_REGISTER({{Register::A, 0}, {Register::D, 1}, {Register::FC, 1}});
}

TEST_F(OpcodeHandlersTest, Sbc8BitE) {
  SetRegisterState({{Register::A, 2}, {Register::E, 3}, {Register::FC, 1}});
  EXPECT_EQ(0, instruction_ptr());
  EXPECT_EQ(4, ExecuteInstruction(static_cast<unsigned char>(0x9B)));
  EXPECT_EQ(1, instruction_ptr());
  EXPECT_REGISTER({{Register::A, 254}, {Register::E, 3}, {Register::FC, 0}});
}

TEST_F(OpcodeHandlersTest, Sbc8BitH) {
  SetRegisterState({{Register::A, 2}, {Register::H, 1}, {Register::FC, 1}});
  EXPECT_EQ(0, instruction_ptr());
  EXPECT_EQ(4, ExecuteInstruction(static_cast<unsigned char>(0x9C)));
  EXPECT_EQ(1, instruction_ptr());
  EXPECT_REGISTER({{Register::A, 0}, {Register::H, 1}, {Register::FC, 1}});
}

TEST_F(OpcodeHandlersTest, Sbc8BitL) {
  SetRegisterState({{Register::A, 200}, {Register::L, 100}, {Register::FC, 1}});
  EXPECT_EQ(0, instruction_ptr());
  EXPECT_EQ(4, ExecuteInstruction(static_cast<unsigned char>(0x9D)));
  EXPECT_EQ(1, instruction_ptr());
  EXPECT_REGISTER({{Register::A, 99}, {Register::L, 100}, {Register::FC, 1}});
}

TEST_F(OpcodeHandlersTest, Sbc8BitHL) {
  SetRegisterState({{Register::A, 2}, {Register::HL, 0xC015}, {Register::FC, 1}});
  SetMemoryState({{0xC015, 1}});
  EXPECT_EQ(0, instruction_ptr());
  EXPECT_EQ(8, ExecuteInstruction(static_cast<unsigned char>(0x9E)));
  EXPECT_EQ(1, instruction_ptr());
  EXPECT_REGISTER({{Register::A, 0}, {Register::HL, 0xC015}, {Register::FC, 1}});
}

TEST_F(OpcodeHandlersTest, Sbc8BitLiteral) {
  SetRegisterState({{Register::A, 2}, {Register::FC, 1}});
  EXPECT_EQ(0, instruction_ptr());
  EXPECT_EQ(8, ExecuteInstruction(static_cast<unsigned char>(0xDE), static_cast<unsigned char>(5)));
  EXPECT_EQ(2, instruction_ptr());
  EXPECT_REGISTER({{Register::A, 252}, {Register::FC, 0}});
}

// Test AND n
// Logically AND n with A, result in A

TEST_F(OpcodeHandlersTest, And8BitA) {
  SetRegisterState({{Register::A, 1}});
  ExecuteInstruction(static_cast<unsigned char>(0xA7));
  EXPECT_REGISTER({{Register::A, 1}});
}

TEST_F(OpcodeHandlersTest, And8BitB) {
  SetRegisterState({{Register::A, 1}, {Register::B, 2}});
  ExecuteInstruction(static_cast<unsigned char>(0xA0));
  EXPECT_REGISTER({{Register::A, 0}, {Register::B, 2}});
  
  SetRegisterState({{Register::A, 2}, {Register::B, 2}});
  ExecuteInstruction(static_cast<unsigned char>(0xA0));
  EXPECT_REGISTER({{Register::A, 2}, {Register::B, 2}});
  
  SetRegisterState({{Register::A, 0}, {Register::B, 0}});
  ExecuteInstruction(static_cast<unsigned char>(0xA0));
  EXPECT_REGISTER({{Register::A, 0}, {Register::B, 0}});
}

TEST_F(OpcodeHandlersTest, And8BitC) {
  SetRegisterState({{Register::A, 2}, {Register::C, 2}});
  EXPECT_EQ(4, ExecuteInstruction(static_cast<unsigned char>(0xA1)));
  EXPECT_REGISTER({{Register::A, 2}, {Register::C, 2}});
}

TEST_F(OpcodeHandlersTest, And8BitD) {
  SetRegisterState({{Register::A, 1}, {Register::D, 2}});
  EXPECT_EQ(4, ExecuteInstruction(static_cast<unsigned char>(0xA2)));
  EXPECT_REGISTER({{Register::A, 0}, {Register::D, 2}});
}

TEST_F(OpcodeHandlersTest, And8BitE) {
  SetRegisterState({{Register::A, 0}, {Register::E, 0}});
  EXPECT_EQ(4, ExecuteInstruction(static_cast<unsigned char>(0xA3)));
  EXPECT_REGISTER({{Register::A, 0}, {Register::E, 0}});
}

TEST_F(OpcodeHandlersTest, And8BitH) {
  SetRegisterState({{Register::A, 2}, {Register::H, 9}});
  EXPECT_EQ(4, ExecuteInstruction(static_cast<unsigned char>(0xA4)));
  EXPECT_REGISTER({{Register::A, 0}, {Register::H, 9}});
}

TEST_F(OpcodeHandlersTest, And8BitL) {
  SetRegisterState({{Register::A, 100}, {Register::L, 20}});
  EXPECT_EQ(4, ExecuteInstruction(static_cast<unsigned char>(0xA5)));
  EXPECT_REGISTER({{Register::A, 4}, {Register::L, 20}});
}

TEST_F(OpcodeHandlersTest, And8BitHL) {
  SetRegisterState({{Register::A, 50}, {Register::HL, 0xC015}});
  SetMemoryState({{0xC015, 3}});
  EXPECT_EQ(8, ExecuteInstruction(static_cast<unsigned char>(0xA6)));
  EXPECT_REGISTER({{Register::A, 2}, {Register::HL, 0xC015}});
}

TEST_F(OpcodeHandlersTest, And8BitLiteral) {
  SetRegisterState({{Register::A, 50}});
  EXPECT_EQ(8, ExecuteInstruction(static_cast<unsigned char>(0xE6), static_cast<unsigned char>(53)));
  EXPECT_REGISTER({{Register::A, 48}});
}

// Test OR n
// Logical OR n with A, result in A

TEST_F(OpcodeHandlersTest, Or8BitA) {
  SetRegisterState({{Register::A, 1}});
  EXPECT_EQ(4, ExecuteInstruction(static_cast<unsigned char>(0xB7)));
  EXPECT_REGISTER({{Register::A, 1}});
}

TEST_F(OpcodeHandlersTest, Or8BitB) {
  SetRegisterState({{Register::A, 1}, {Register::B, 2}});
  EXPECT_EQ(4, ExecuteInstruction(static_cast<unsigned char>(0xB0)));
  EXPECT_REGISTER({{Register::A, 3}, {Register::B, 2}});
}

TEST_F(OpcodeHandlersTest, Or8BitC) {
  SetRegisterState({{Register::A, 2}, {Register::C, 2}});
  EXPECT_EQ(4, ExecuteInstruction(static_cast<unsigned char>(0xB1)));
  EXPECT_REGISTER({{Register::A, 2}, {Register::C, 2}});
}

TEST_F(OpcodeHandlersTest, Or8BitD) {
  SetRegisterState({{Register::A, 1}, {Register::D, 0}});
  EXPECT_EQ(4, ExecuteInstruction(static_cast<unsigned char>(0xB2)));
  EXPECT_REGISTER({{Register::A, 1}, {Register::D, 0}});
}

TEST_F(OpcodeHandlersTest, Or8BitE) {
  SetRegisterState({{Register::A, 50}, {Register::E, 30}});
  EXPECT_EQ(4, ExecuteInstruction(static_cast<unsigned char>(0xB3)));
  EXPECT_REGISTER({{Register::A, 62}, {Register::E, 30}});
}

TEST_F(OpcodeHandlersTest, Or8BitH) {
  SetRegisterState({{Register::A, 0}, {Register::H, 255}});
  EXPECT_EQ(4, ExecuteInstruction(static_cast<unsigned char>(0xB4)));
  EXPECT_REGISTER({{Register::A, 255}, {Register::H, 255}});
}

TEST_F(OpcodeHandlersTest, Or8BitL) {
  SetRegisterState({{Register::A, 0}, {Register::L, 0}});
  EXPECT_EQ(4, ExecuteInstruction(static_cast<unsigned char>(0xB5)));
  EXPECT_REGISTER({{Register::A, 0}, {Register::L, 0}});
}

TEST_F(OpcodeHandlersTest, Or8BitHL) {
  SetRegisterState({{Register::A, 29}, {Register::HL, 0xC015}});
  SetMemoryState({{0xC015, 0}});
  EXPECT_EQ(8, ExecuteInstruction(static_cast<unsigned char>(0xB6)));
  EXPECT_REGISTER({{Register::A, 29}, {Register::HL, 0xC015}});
}

TEST_F(OpcodeHandlersTest, Or8BitLiteral) {
  SetRegisterState({{Register::A, 29}});
  EXPECT_EQ(8, ExecuteInstruction(static_cast<unsigned char>(0xF6), static_cast<unsigned char>(27)));
  EXPECT_REGISTER({{Register::A, 31}});
}

// Test XOR n
//  Logical exclusive OR n with register A, result in A.

TEST_F(OpcodeHandlersTest, Xor8BitA) {
  SetRegisterState({{Register::A, 10}});
  EXPECT_EQ(4, ExecuteInstruction(static_cast<unsigned char>(0xAF)));
  EXPECT_REGISTER({{Register::A, 0}});
}

TEST_F(OpcodeHandlersTest, Xor8BitB) {
  SetRegisterState({{Register::A, 1}, {Register::B, 2}});
  ExecuteInstruction(static_cast<unsigned char>(0xA8));
  EXPECT_REGISTER({{Register::A, 3}, {Register::B, 2}});
}

TEST_F(OpcodeHandlersTest, Xor8BitC) {
  SetRegisterState({{Register::A, 2}, {Register::C, 2}});
  EXPECT_EQ(4, ExecuteInstruction(static_cast<unsigned char>(0xA9)));
  EXPECT_REGISTER({{Register::A, 0}, {Register::C, 2}});
}

TEST_F(OpcodeHandlersTest, Xor8BitD) {
  SetRegisterState({{Register::A, 2}, {Register::D, 3}});
  EXPECT_EQ(4, ExecuteInstruction(static_cast<unsigned char>(0xAA)));
  EXPECT_REGISTER({{Register::A, 1}, {Register::D, 3}});
}

TEST_F(OpcodeHandlersTest, Xor8BitE) {
  SetRegisterState({{Register::A, 1}, {Register::E, 0}});
  EXPECT_EQ(4, ExecuteInstruction(static_cast<unsigned char>(0xAB)));
  EXPECT_REGISTER({{Register::A, 1}, {Register::E, 0}});
}

TEST_F(OpcodeHandlersTest, Xor8BitH) {
  SetRegisterState({{Register::A, 2}, {Register::H, 9}});
  EXPECT_EQ(4, ExecuteInstruction(static_cast<unsigned char>(0xAC)));
  EXPECT_REGISTER({{Register::A, 11}, {Register::H, 9}});
}

TEST_F(OpcodeHandlersTest, Xor8BitL) {
  SetRegisterState({{Register::A, 20}, {Register::L, 10}});
  EXPECT_EQ(4, ExecuteInstruction(static_cast<unsigned char>(0xAD)));
  EXPECT_REGISTER({{Register::A, 30}, {Register::L, 10}});
}

TEST_F(OpcodeHandlersTest, Xor8BitHL) {
  SetRegisterState({{Register::A, 2}, {Register::HL, 0xC015}});
  SetMemoryState({{0xC015, 63}});
  EXPECT_EQ(8, ExecuteInstruction(static_cast<unsigned char>(0xAE)));
  EXPECT_REGISTER({{Register::A, 61}, {Register::HL, 0xC015}});
}

TEST_F(OpcodeHandlersTest, Xor8BitLiteral) {
  SetRegisterState({{Register::A, 2}});
  EXPECT_EQ(8, ExecuteInstruction(static_cast<unsigned char>(0xEE), static_cast<unsigned char>(10)));
  EXPECT_REGISTER({{Register::A, 8}});
}

// Tests for CP n
// Compare A with n. Basically does A - n subtraction with results thrown away

TEST_F(OpcodeHandlersTest, Cp8BitA) {
  SetRegisterState({{Register::A, 1}});
  EXPECT_EQ(4, ExecuteInstruction(static_cast<unsigned char>(0xBF)));
  EXPECT_REGISTER({{Register::FZ, 1}, {Register::FN, 1}, {Register::FH, 0}, {Register::FC, 0}});
}

TEST_F(OpcodeHandlersTest, Cp8BitB) {
  SetRegisterState({{Register::A, 1}, {Register::B, 2}});
  EXPECT_EQ(4, ExecuteInstruction(static_cast<unsigned char>(0xB8)));
  EXPECT_REGISTER({{Register::FZ, 0}, {Register::FN, 1}, {Register::FH, 1}, {Register::FC, 1}});
}

TEST_F(OpcodeHandlersTest, Cp8BitC) {
  SetRegisterState({{Register::A, 4}, {Register::C, 4}});
  EXPECT_EQ(4, ExecuteInstruction(static_cast<unsigned char>(0xB9)));
  EXPECT_REGISTER({{Register::FZ, 1}, {Register::FN, 1}, {Register::FH, 0}, {Register::FC, 0}});
}

TEST_F(OpcodeHandlersTest, Cp8BitD) {
  SetRegisterState({{Register::A, 1}, {Register::D, 2}});
  EXPECT_EQ(4, ExecuteInstruction(static_cast<unsigned char>(0xBA)));
  EXPECT_REGISTER({{Register::FZ, 0}, {Register::FN, 1}, {Register::FH, 1}, {Register::FC, 1}});
}

TEST_F(OpcodeHandlersTest, Cp8BitE) {
  SetRegisterState({{Register::A, 1}, {Register::E, 2}});
  EXPECT_EQ(4, ExecuteInstruction(static_cast<unsigned char>(0xBB)));
  EXPECT_REGISTER({{Register::FZ, 0}, {Register::FN, 1}, {Register::FH, 1}, {Register::FC, 1}});
}

TEST_F(OpcodeHandlersTest, Cp8BitH) {
  SetRegisterState({{Register::A, 1}, {Register::H, 2}});
  EXPECT_EQ(4, ExecuteInstruction(static_cast<unsigned char>(0xBC)));
  EXPECT_REGISTER({{Register::FZ, 0}, {Register::FN, 1}, {Register::FH, 1}, {Register::FC, 1}});
}

TEST_F(OpcodeHandlersTest, Cp8BitL) {
  SetRegisterState({{Register::A, 1}, {Register::L, 2}});
  EXPECT_EQ(4, ExecuteInstruction(static_cast<unsigned char>(0xBD)));
  EXPECT_REGISTER({{Register::FZ, 0}, {Register::FN, 1}, {Register::FH, 1}, {Register::FC, 1}});
}

TEST_F(OpcodeHandlersTest, Cp8BitHL) {
  SetRegisterState({{Register::A, 1}, {Register::HL, 0xC015}});
  SetMemoryState({{0xC015, 2}});
  EXPECT_EQ(8, ExecuteInstruction(static_cast<unsigned char>(0xBE)));
  EXPECT_REGISTER({{Register::FZ, 0}, {Register::FN, 1}, {Register::FH, 1}, {Register::FC, 1}});
}

TEST_F(OpcodeHandlersTest, Cp8BitLiteral) {
  SetRegisterState({{Register::A, 1}});
  EXPECT_EQ(8, ExecuteInstruction(static_cast<unsigned char>(0xFE), static_cast<unsigned char>(2)));
  EXPECT_REGISTER({{Register::FZ, 0}, {Register::FN, 1}, {Register::FH, 1}, {Register::FC, 1}});
}

// Test INC n
// Increment register n

TEST_F(OpcodeHandlersTest, INC8BitA) {
  SetRegisterState({{Register::A, 1}});
  EXPECT_EQ(4, ExecuteInstruction(static_cast<unsigned char>(0x3C)));
  EXPECT_REGISTER({{Register::A, 2}});
}

TEST_F(OpcodeHandlersTest, INC8BitB) {
  SetRegisterState({{Register::B, 4}});
  EXPECT_EQ(4, ExecuteInstruction(static_cast<unsigned char>(0x04)));
  EXPECT_REGISTER({{Register::B, 5}});
}

TEST_F(OpcodeHandlersTest, INC8BitC) {
  SetRegisterState({{Register::C, 1}});
  EXPECT_EQ(4, ExecuteInstruction(static_cast<unsigned char>(0x0c)));
  EXPECT_REGISTER({{Register::C, 2}});
}

TEST_F(OpcodeHandlersTest, INC8BitD) {
  SetRegisterState({{Register::D, 0}});
  EXPECT_EQ(4, ExecuteInstruction(static_cast<unsigned char>(0x14)));
  EXPECT_REGISTER({{Register::D, 1}});
}

TEST_F(OpcodeHandlersTest, INC8BitE) {
  SetRegisterState({{Register::E, 255}});
  EXPECT_EQ(4, ExecuteInstruction(static_cast<unsigned char>(0x1c)));
  EXPECT_REGISTER({{Register::E, 0}, {Register::FZ, 1}});
}

TEST_F(OpcodeHandlersTest, INC8BitH) {
  SetRegisterState({{Register::H, 7}});
  EXPECT_EQ(4, ExecuteInstruction(static_cast<unsigned char>(0x24)));
  EXPECT_REGISTER({{Register::H, 8}, {Register::FH, 1}});
}

TEST_F(OpcodeHandlersTest, INC8BitL) {
  SetRegisterState({{Register::L, 1}});
  EXPECT_EQ(4, ExecuteInstruction(static_cast<unsigned char>(0x2C)));
  EXPECT_REGISTER({{Register::L, 2}});
}

TEST_F(OpcodeHandlersTest, INC8BitHL) {
  SetRegisterState({{Register::HL, 0xC015}});
  SetMemoryState({{0xC015, 1}});
  EXPECT_EQ(12, ExecuteInstruction(static_cast<unsigned char>(0x34)));
  EXPECT_REGISTER({{Register::HL, 0xC015}, {Register::FN, 0}});
  EXPECT_MEMORY({{0xC015, 2}});
}

// Test DEC n
// Decrement register n

TEST_F(OpcodeHandlersTest, Dec8BitA) {
  SetRegisterState({{Register::A, 1}});
  EXPECT_EQ(4, ExecuteInstruction(static_cast<unsigned char>(0x3D)));
  EXPECT_REGISTER({{Register::A, 0}});
}

TEST_F(OpcodeHandlersTest, Dec8BitB) {
  SetRegisterState({{Register::B, 9}});
  EXPECT_EQ(4, ExecuteInstruction(static_cast<unsigned char>(0x05)));
  EXPECT_REGISTER({{Register::B, 8}});
}

TEST_F(OpcodeHandlersTest, Dec8BitC) {
  SetRegisterState({{Register::C, 15}});
  EXPECT_EQ(4, ExecuteInstruction(static_cast<unsigned char>(0x0D)));
  EXPECT_REGISTER({{Register::C, 14}, {Register::FH, 0}});
}

TEST_F(OpcodeHandlersTest, Dec8BitD) {
  SetRegisterState({{Register::D, 3}});
  EXPECT_EQ(4, ExecuteInstruction(static_cast<unsigned char>(0x15)));
  EXPECT_REGISTER({{Register::D, 2}});
}

TEST_F(OpcodeHandlersTest, Dec8BitE) {
  SetRegisterState({{Register::E, 1}});
  EXPECT_EQ(4, ExecuteInstruction(static_cast<unsigned char>(0x1D)));
  EXPECT_REGISTER({{Register::E, 0}, {Register::FZ, 1}});
}

TEST_F(OpcodeHandlersTest, Dec8BitH) {
  SetRegisterState({{Register::H, 4}});
  EXPECT_EQ(4, ExecuteInstruction(static_cast<unsigned char>(0x25)));
  EXPECT_REGISTER({{Register::H, 3}});
}

TEST_F(OpcodeHandlersTest, Dec8BitL) {
  SetRegisterState({{Register::L, 2}});
  EXPECT_EQ(4, ExecuteInstruction(static_cast<unsigned char>(0x2D)));
  EXPECT_REGISTER({{Register::L, 1}, {Register::FN, 1}});
}

TEST_F(OpcodeHandlersTest, Dec8BitHL) {
  SetRegisterState({{Register::HL, 0xC015}});
  SetMemoryState({{0xC015, 2}});
  EXPECT_EQ(12, ExecuteInstruction(static_cast<unsigned char>(0x35)));
  EXPECT_REGISTER({{Register::HL, 0xC015}});
  EXPECT_MEMORY({{0xC015, 1}});
}

// End 8-bit ALU Tests

// Begin 16-bit ALU Tests

// Tests for ADD HL,n
// Add n to HL

TEST_F(OpcodeHandlersTest, Add16BitHLBC) {
  SetRegisterState({{Register::HL, 1}, {Register::BC, 2}});
  EXPECT_EQ(0, instruction_ptr());
  EXPECT_EQ(8, ExecuteInstruction(static_cast<unsigned char>(0x09)));
  EXPECT_EQ(1, instruction_ptr());
  EXPECT_REGISTER({{Register::HL, 3}, {Register::BC, 2}});
}

TEST_F(OpcodeHandlersTest, Add16BitHLDE) {
  SetRegisterState({{Register::HL, 0xFFFF}, {Register::DE, 0x0001}, {Register::FC, 0}});
  EXPECT_EQ(0, instruction_ptr());
  EXPECT_EQ(8, ExecuteInstruction(static_cast<unsigned char>(0x19)));
  EXPECT_EQ(1, instruction_ptr());
  EXPECT_REGISTER({{Register::HL, 0x0}, {Register::DE, 0x0001}, {Register::FC, 1}});
}

TEST_F(OpcodeHandlersTest, Add16BitHLHL) {
  SetRegisterState({{Register::HL, 0x1}});
  EXPECT_EQ(0, instruction_ptr());
  EXPECT_EQ(8, ExecuteInstruction(static_cast<unsigned char>(0x29)));
  EXPECT_EQ(1, instruction_ptr());
  EXPECT_REGISTER({{Register::HL, 0x2}});
}

TEST_F(OpcodeHandlersTest, Add16BitHLSP) {
  SetRegisterState({{Register::HL, 1}, {Register::SP, 2}});
  EXPECT_EQ(0, instruction_ptr());
  EXPECT_EQ(8, ExecuteInstruction(static_cast<unsigned char>(0x39)));
  EXPECT_EQ(1, instruction_ptr());
  EXPECT_REGISTER({{Register::HL, 3}, {Register::SP, 2}});
}

// Tests for ADD SP,n
// Add n to Stack Pointer (SP)

TEST_F(OpcodeHandlersTest, Add16BitSPLiteral1) {
  SetRegisterState({{Register::SP, 0xFFFF}});
  EXPECT_EQ(0, instruction_ptr());
  EXPECT_EQ(16, ExecuteInstruction(static_cast<unsigned char>(0xE8), static_cast<unsigned char>(0xA)));
  EXPECT_EQ(2, instruction_ptr());
  EXPECT_REGISTER({{Register::SP, 0x09}, {Register::FC, 1}});
}

TEST_F(OpcodeHandlersTest, Add16BitSPLiteral2) {
  SetRegisterState({{Register::SP, 0x05}});
  EXPECT_EQ(0, instruction_ptr());
  EXPECT_EQ(16, ExecuteInstruction(static_cast<unsigned char>(0xE8), static_cast<unsigned char>(0xF0)));
  EXPECT_EQ(2, instruction_ptr());
  EXPECT_REGISTER({{Register::SP, 0xF5}});
}

// Test INC nn
// Increment register nn

TEST_F(OpcodeHandlersTest, INC16BitBC) {
  SetRegisterState({{Register::BC, 0xFE}});
  EXPECT_EQ(8, ExecuteInstruction(static_cast<unsigned char>(0x03)));
  EXPECT_REGISTER({{Register::BC, 0xFF}});
}

TEST_F(OpcodeHandlersTest, INC16BitDE) {
  SetRegisterState({{Register::DE, 0xFFFF}});
  EXPECT_EQ(8, ExecuteInstruction(static_cast<unsigned char>(0x13)));
  EXPECT_REGISTER({{Register::DE, 0x0}});
}

TEST_F(OpcodeHandlersTest, INC16BitHL) {
  SetRegisterState({{Register::HL, 0x0}});
  EXPECT_EQ(8, ExecuteInstruction(static_cast<unsigned char>(0x23)));
  EXPECT_REGISTER({{Register::HL, 0x1}});
}

TEST_F(OpcodeHandlersTest, INC16BitSP) {
  SetRegisterState({{Register::SP, 0xFFFE}});
  EXPECT_EQ(8, ExecuteInstruction(static_cast<unsigned char>(0x33)));
  EXPECT_REGISTER({{Register::SP, 0xFFFF}});
}


// Test DEC nn
// Decerement register nn

TEST_F(OpcodeHandlersTest, Dec16BitBC) {
  SetRegisterState({{Register::BC, 0xFE}});
  EXPECT_EQ(8, ExecuteInstruction(static_cast<unsigned char>(0x0B)));
  EXPECT_REGISTER({{Register::BC, 0xFD}});
}

TEST_F(OpcodeHandlersTest, Dec16BitDE) {
  SetRegisterState({{Register::DE, 0xFFFF}});
  EXPECT_EQ(8, ExecuteInstruction(static_cast<unsigned char>(0x1B)));
  EXPECT_REGISTER({{Register::DE, 0xFFFE}});
}

TEST_F(OpcodeHandlersTest, Dec16BitHL) {
  SetRegisterState({{Register::HL, 0x0}});
  EXPECT_EQ(8, ExecuteInstruction(static_cast<unsigned char>(0x2B)));
  EXPECT_REGISTER({{Register::HL, 0xFFFF}});
}

TEST_F(OpcodeHandlersTest, Dec16BitSP) {
  SetRegisterState({{Register::SP, 0xF}});
  EXPECT_EQ(8, ExecuteInstruction(static_cast<unsigned char>(0x3B)));
  EXPECT_REGISTER({{Register::SP, 0xE}});
}

// End 16-bit ALU tests

// Miscelaneous
  
TEST_F(OpcodeHandlersTest, SWAPA) {
  SetRegisterState({{Register::A, 0x12}});
  EXPECT_EQ(8, ExecuteInstruction(static_cast<unsigned short>(0xCB37)));
  EXPECT_REGISTER({{Register::A, 0x21}});
}

TEST_F(OpcodeHandlersTest, SWAPB) {
  SetRegisterState({{Register::B, 0x12}});
  EXPECT_EQ(8, ExecuteInstruction(static_cast<unsigned short>(0xCB30)));
  EXPECT_REGISTER({{Register::B, 0x21}});
}

TEST_F(OpcodeHandlersTest, SWAPC) {
  SetRegisterState({{Register::C, 0x12}});
  EXPECT_EQ(8, ExecuteInstruction(static_cast<unsigned short>(0xCB31)));
  EXPECT_REGISTER({{Register::C, 0x21}});
}

TEST_F(OpcodeHandlersTest, SWAPD) {
  SetRegisterState({{Register::D, 0x12}});
  EXPECT_EQ(8, ExecuteInstruction(static_cast<unsigned short>(0xCB32)));
  EXPECT_REGISTER({{Register::D, 0x21}});
}

TEST_F(OpcodeHandlersTest, SWAPE) {
  SetRegisterState({{Register::E, 0x12}});
  EXPECT_EQ(8, ExecuteInstruction(static_cast<unsigned short>(0xCB33)));
  EXPECT_REGISTER({{Register::E, 0x21}});
}

TEST_F(OpcodeHandlersTest, SWAPH) {
  SetRegisterState({{Register::H, 0x12}});
  EXPECT_EQ(8, ExecuteInstruction(static_cast<unsigned short>(0xCB34)));
  EXPECT_REGISTER({{Register::H, 0x21}});
}

TEST_F(OpcodeHandlersTest, SWAPL) {
  SetRegisterState({{Register::L, 0x12}});
  EXPECT_EQ(8, ExecuteInstruction(static_cast<unsigned short>(0xCB35)));
  EXPECT_REGISTER({{Register::L, 0x21}});
}

TEST_F(OpcodeHandlersTest, SWAPHL) {
  SetRegisterState({{Register::HL, 0xC020}});
  SetMemoryState({{0xC020, 0x12}});
  EXPECT_EQ(16, ExecuteInstruction(static_cast<unsigned short>(0xCB36)));
  EXPECT_REGISTER({{Register::HL, 0xC020}});
  EXPECT_MEMORY({{0xC020, 0x21}});
}
  
TEST_F(OpcodeHandlersTest, DAA) {
  SetRegisterState({{Register::A, 0x32}});
  EXPECT_EQ(4, ExecuteInstruction(static_cast<unsigned char>(0x27)));
  EXPECT_REGISTER({{Register::A, 0x50}});
}
  
TEST_F(OpcodeHandlersTest, CPL) {
  SetRegisterState({{Register::A, 0x44}});
  EXPECT_EQ(4, ExecuteInstruction(static_cast<unsigned char>(0x2F)));
  EXPECT_REGISTER({{Register::A, 0xBB}, {Register::FN, 1}, {Register::FH, 1}});
}
  
TEST_F(OpcodeHandlersTest, CCF) {
  SetRegisterState({{Register::FC, 1}, {Register::FN, 1}, {Register::FZ, 1}, {Register::FH, 1}});
  EXPECT_EQ(4, ExecuteInstruction(static_cast<unsigned char>(0x3F)));
  EXPECT_REGISTER({{Register::FC, 0}, {Register::FN, 0}, {Register::FZ, 1}, {Register::FH, 0}});
}

TEST_F(OpcodeHandlersTest, SCF) {
  SetRegisterState({{Register::FC, 0}, {Register::FN, 1}, {Register::FZ, 1}, {Register::FH, 1}});
  EXPECT_EQ(4, ExecuteInstruction(static_cast<unsigned char>(0x37)));
  EXPECT_REGISTER({{Register::FC, 1}, {Register::FN, 0}, {Register::FZ, 1}, {Register::FH, 0}});
}
  
// End Miscellaneous
  
TEST_F(OpcodeHandlersTest, Call) {
  SetRegisterState({{Register::SP, 0xfffe}, {Register::PC, 0x1234}});
  ExecuteInstruction(static_cast<unsigned char>(0xcd), static_cast<unsigned short>(0x4523));
  EXPECT_REGISTER({{Register::SP, 0xfffc}, {Register::PC, 0x2345}});
  EXPECT_MEMORY({{0xfffe, 0x35}, {0xfffd, 0x12}});
}

TEST_F(OpcodeHandlersTest, LoadAndRunROM) {
  SetRegisterState({{Register::PC, 0x0100}, {Register::B, 0x01}});
  LoadAndRunROM({{0x0100, {
    0x00, // NOP
    0x80, // ADD A, B
  }}});
  EXPECT_REGISTER({{Register::PC, 0x0102}, {Register::A, 0x01}});
}

TEST_F(OpcodeHandlersTest, Interrupt) {
  SetRegisterState({{Register::SP, 0xfffe}, {Register::PC, 0x0100}, {Register::B, 0x01}});
  // TODO(Brendan): This state should be set directly and not depend on an
  // instruction.
  ExecuteInstruction(static_cast<unsigned char>(0xfb)); // EI (Set IME).
  SetMemoryState({{0xffff, 0b00011111}, {0xff0f, 0b00000001}});
  LoadROM({{0x0101, {
    0x80,
    0x80,
  }}, {0x0040, {
    0x90,
    0x90}}});
  Run(1);
  EXPECT_REGISTER({{Register::SP, 0xfffc}, {Register::PC, 0x0041}, {Register::A, 0xff}});
  // TODO(Brendan): Should also have assertions for the state of the interrupt
  // flags.
}
  
} // namespace handlers
} // namespace back_end
