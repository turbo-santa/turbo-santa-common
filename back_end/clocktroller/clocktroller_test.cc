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
    clocktroller->Launch();
    printf("Launched\n");
    //sleep(1);
    //clocktroller->Terminate();
    EXPECT_REGISTER({{Register::A, 7}});
}

} // namespace clocktroller
} // namespace back_end