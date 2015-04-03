#ifndef TURBO_SANTA_COMMON_BACK_END_CLOCKTROLLER_CLOCKTROLLER_TEST_H
#define TURBO_SANTA_COMMON_BACK_END_CLOCKTROLLER_CLOCKTROLLER_TEST_H

#include "test_harness/test_harness.h"
#include "test_harness/test_harness_utils.h"

namespace back_end {
namespace memory {
    class MemoryMapper;
} // namespace memory
} // namespace back_end

namespace back_end {
namespace handlers {
  class OpcodeExecutor;
} // namespace opcode_executor
} // namespace back_end

namespace back_end {
namespace clocktroller {

handlers::OpcodeExecutor* parser = new handlers::OpcodeExecutor();

class ClocktrollerTest : public test_harness::TestHarness {
    protected:
        ClocktrollerTest() : test_harness::TestHarness(parser) {}
        void LoadROM(const std::vector<test_harness::TestROM>& test_rom);
};

} // namespace clocktroller
} // namespace back_end
#endif // TURBO_SANTA_COMMON_BACK_END_CLOCKTROLLER_CLOCKTROLLER_TEST_H
