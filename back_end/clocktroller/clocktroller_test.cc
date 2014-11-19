#include "clocktroller_test.h"
#include "clocktroller.h"
#include "back_end/opcode_executor/registers.h"
#include "third_party/gtest/include/gtest/gtest.h"
#include <glog/logging.h>
#include <vector>

namespace back_end {
namespace clocktroller {

using Register = test_harness::RegisterNameValuePair;
using handlers::OpcodeExecutor;
using test_harness::TestHarness;
using test_harness::TestROM;
using std::vector;
using memory::MBC;

void ClocktrollerTest::LoadROM(const vector<TestROM>& test_rom) {
  MBC* mbc = parser_->memory_mapper_.mbc_.get();
  for (const TestROM& segment : test_rom) {
    unsigned short address = segment.start_address;
    for (unsigned char instruction : segment.instructions) {
      mbc->ForceWrite(address, instruction);
      address++;
    }
  }
}

TEST_F(ClocktrollerTest, ExecuteInstructions) {
    LOG(INFO) << "Setting register state";
    SetRegisterState({{Register::A, 1}, {Register::L, 5}, {Register::H, 3}});
    // add l to a
    // add 8 bit literal to a
    // sub h from a
    LOG(INFO) << "Creating rom";
    LoadROM({{0x0000, {
        0x85,       // ADD A, L
    //     0xC6, 0x04, // ADD A, 0x04
        0x94}       // SUB H
    }});
    
    Clocktroller* clocktroller = new Clocktroller(nullptr, 3);
    LOG(INFO) << "Created new clocktroller";
    clocktroller->executor = parser_;
    LOG(INFO) << "Set OpcodeExecutor";
    clocktroller->Setup();
    LOG(INFO) << "Clocktroller set up";
    clocktroller->Start();
    LOG(INFO) << "Launched";
    clocktroller->WaitForThreads();
    EXPECT_REGISTER({{Register::A, 3}});
}

TEST_F(ClocktrollerTest, DISABLED_InstructionRuntime) {
    SetRegisterState({{Register::A, 1}, {Register::B, 1}});
    // add B to A repeatedly
    // each add should take 4 clock cycles
    // 8388000 cycles/sec 
    int numInstructions = 30000;
    std::vector<unsigned char> instructions;
    for (int i = 0; i < numInstructions; i++) {
        instructions.push_back(0x80);
    }
    LOG(INFO) << "Creating rom";
    LoadROM({{0x0000, instructions}});

    Clocktroller* clocktroller = new Clocktroller(nullptr, numInstructions);
    LOG(INFO) << "Created new clocktroller";
    clocktroller->executor = parser_;
    LOG(INFO) << "Set OpcodeExecutor";
    clocktroller->Setup();
    LOG(INFO) << "Clocktroller set up";
    clock_t start = clock();
    clocktroller->Start();
    LOG(INFO) << "Launched";
    clocktroller->WaitForThreads();
    clock_t stop = clock();

    EXPECT_EQ(((float)4 * numInstructions) /  8388000, ((float)stop - start) / CLOCKS_PER_SEC);
}


} // namespace clocktroller
} // namespace back_end


