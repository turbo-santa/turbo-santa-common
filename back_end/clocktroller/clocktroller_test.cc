#include "clocktroller.h"
#include "back_end/opcode_executor/opcode_executor.h"
#include "back_end/opcode_executor/registers.h"
#include "back_end/memory/memory_mapper.h"
#include "test_harness/test_harness.h"
#include "test_harness/test_harness_utils.h"
#include "third_party/gtest/include/gtest/gtest.h"

namespace back_end {
namespace clocktroller {

using Register = test_harness::RegisterNameValuePair;
using test_harness::TestHarness;
using handlers::OpcodeExecutor;

OpcodeExecutor* parser = new OpcodeExecutor(nullptr, 0);

class ClocktrollerTest : public test_harness::TestHarness {
    protected:
        ClocktrollerTest() : test_harness::TestHarness(parser) {}
};

TEST_F(ClocktrollerTest, ExecuteInstructions) {
    SetRegisterState({{Register::A, 1}});
    SetRegisterState({{Register::L, 5}});
    SetRegisterState({{Register::H, 3}});
    // add l to a
    // add 8 bit literal to a
    // sub h from a
    unsigned char rom[4] = {0x85, 0xC6, 0x04, 0x94};
    Clocktroller* clocktroller = new Clocktroller(rom, 3);
    printf("Created new clocktroller\n");
    clocktroller->Start();
    printf("Launched\n");
    clocktroller->WaitForThreads();
    EXPECT_REGISTER({{Register::A, 7}});
}

TEST_F(ClocktrollerTest, InstructionRuntime) {
    SetRegisterState({{Register::A, 1}});
    SetRegisterState({{Register::B, 1}});
    // add B to A repeatedly
    // each add should take 4 clock cycles
    // 8388000 cycles/sec 
    int numInstructions = 100000;
    unsigned char rom[numInstructions];
    for (int i = 0; i < numInstructions; i++) {
        rom[i] = 0x80;
    }

    Clocktroller* clocktroller = new Clocktroller(rom, numInstructions);
    clock_t start = clock();
    clocktroller->Start();
    clocktroller->WaitForThreads();
    clock_t stop = clock();

    // TODO: fix this value
    EXPECT_EQ(((float)4 * numInstructions) /  8388000, ((float)stop - start) / CLOCKS_PER_SEC);
}


} // namespace clocktroller
} // namespace back_end


