#ifndef TURBO_SANTA_TEST_HARNESS_TEST_HARNESS_H_
#define TURBO_SANTA_TEST_HARNESS_TEST_HARNESS_H_

#include <memory>
#include <string>
#include <vector>
#include "back_end/opcode_executor/opcode_executor.h"
#include "test_harness/test_harness_utils.h"
#include "third_party/gtest/include/gtest/gtest.h"

namespace test_harness {

class TestHarness : public ::testing::Test {
    public:
        void SetRegisterState(const std::vector<RegisterNameValuePair>& register_diff);
        void ExecuteInstruction(unsigned char instruction);
        ::testing::AssertionResult AssertRegisterAdapter(
                const std::string& not_used,
                std::vector<RegisterNameValuePair>& register_diff) {
            return AssertRegisterState(register_diff);
        }
        ::testing::AssertionResult AssertRegisterState(const std::vector<RegisterNameValuePair>& register_diff);
        ::testing::AssertionResult AssertMemoryAdapter(
                const std::string& not_used,
                const std::vector<MemoryAddressValuePair>& memory_diff) {
            return AssertMemoryState(memory_diff);
        }
        ::testing::AssertionResult AssertMemoryState(const std::vector<MemoryAddressValuePair>& memory_diff);
        int get_instruction_ptr();

    protected:
        TestHarness(back_end::handlers::OpcodeExecutor* parser) : parser_(parser) {}
        virtual void TearDown() { ClearParser(); }

    private:
        bool VerifyCorrectInstruction(const std::vector<unsigned char>& instruction);
        ::testing::AssertionResult ValidateRegister(const RegisterNameValuePair& register_diff);
        void ClearParser();
        bool SetInitialState(const DiffState& initial_state);
        bool SetRegisterState(const RegisterNameValuePair& register_diff);
        bool LoadROM(const std::vector<InstructionExpectedStatePair>& instructions);
        // TODO(Brendan): TestHarnesses will want to reuse the parser. Do not
        // delete it when done.
        back_end::handlers::OpcodeExecutor* parser_;
};

#define EXPECT_REGISTER(...)\
{\
    std::vector<test_harness::RegisterNameValuePair> reg_list_macro = __VA_ARGS__;\
    EXPECT_PRED_FORMAT1(AssertRegisterAdapter, reg_list_macro);\
}

#define EXPECT_MEMORY(...)\
{\
    std::vector<test_harness::RegisterNameValuePair> reg_list_macro = __VA_ARGS__;\
    EXPECT_PRED_FORMAT1(AssertMemoryAdapter, reg_list_macro);\
}

} // namespace test_harness

#endif // TURBO_SANTA_TEST_HARNESS_TEST_HARNESS_H_
