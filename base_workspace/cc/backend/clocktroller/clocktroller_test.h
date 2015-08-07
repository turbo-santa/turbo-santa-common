#ifndef TURBO_SANTA_COMMON_BACK_END_CLOCKTROLLER_CLOCKTROLLER_TEST_H
#define TURBO_SANTA_COMMON_BACK_END_CLOCKTROLLER_CLOCKTROLLER_TEST_H

#include "cc/test_harness/test_harness.h"
#include "cc/test_harness/test_harness_utils.h"

namespace backend {
namespace memory {
    class MemoryMapper;
} // namespace memory
} // namespace backend

namespace backend {
namespace handlers {
  class OpcodeExecutor;
} // namespace opcode_executor
} // namespace backend

namespace backend {
namespace clocktroller {

handlers::OpcodeExecutor* parser = new handlers::OpcodeExecutor();

class ClocktrollerTest : public cc/test_harness::TestHarness {
    protected:
        ClocktrollerTest() : cc/test_harness::TestHarness(parser) {}
        void LoadROM(const std::vector<cc/test_harness::TestROM>& test_rom);
};

} // namespace clocktroller
} // namespace backend
#endif // TURBO_SANTA_COMMON_BACK_END_CLOCKTROLLER_CLOCKTROLLER_TEST_H
