#include "cc/backend/config.h"

#include "cc/backend/clocktroller/clocktroller_test.h"
#include "cc/backend/clocktroller/clocktroller.h"
#include "cc/backend/opcode_executor/registers.h"
#include "submodules/googletest/include/gtest/gtest.h"
#include "submodules/glog/src/glog/logging.h"
#include <vector>

namespace backend {
namespace clocktroller {
    
using Register = test_harness::RegisterNameValuePair;
using handlers::OpcodeExecutor;
using test_harness::TestHarness;
using test_harness::TestROM;
using std::unique_ptr;
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
    0xC6, 0x04, // ADD A, 0x04
    0x94}       // SUB H
  }});
  
  unique_ptr<Clocktroller> clocktroller(new Clocktroller(nullptr, 3));
  LOG(INFO) << "Created new clocktroller";
  clocktroller->executor = parser_;
  LOG(INFO) << "Set OpcodeExecutor";
  clocktroller->Setup();
  LOG(INFO) << "Clocktroller set up";
  clocktroller->Start();
  LOG(INFO) << "Launched";
  clocktroller->WaitForThreads();
  EXPECT_REGISTER({{Register::A, 7}});
}

TEST_F(ClocktrollerTest, DISABLED_InstructionRuntime) {
  SetRegisterState({{Register::A, 1}, {Register::B, 1}});
  // add B to A repeatedly
  // each add should take 4 clock cycles
  // 8388000 cycles/sec
  float clock_cycles = 4;
  int numInstructions = 30000;
  std::vector<unsigned char> instructions;
  for (int i = 0; i < numInstructions; i++) {
    instructions.push_back(0x80); // add b to a takes 4 clockcycles
  }
  LOG(INFO) << "Creating rom";
  LoadROM({{0x0000, instructions}});
  
  unique_ptr<Clocktroller> clocktroller(new Clocktroller(nullptr, numInstructions));
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
  
  EXPECT_EQ(((float)clock_cycles * numInstructions) /  8388000, ((float)stop - start) / CLOCKS_PER_SEC);
}
  
    
} // namespace clocktroller
} // namespace backend
