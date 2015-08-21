#ifndef TURBO_SANTA_TEST_HARNESS_TEST_HARNESS_H_
#define TURBO_SANTA_TEST_HARNESS_TEST_HARNESS_H_

#include <memory>
#include <string>
#include <vector>
  
#include "cc/backend/opcode_executor/opcode_executor.h"
#include "cc/test_harness/test_harness_utils.h"
#include "include/gtest/gtest.h"

namespace test_harness {

class TestHarness : public ::testing::Test {
    public:
        void SetMemoryState(const std::vector<MemoryAddressValuePair>& memory_diff);
        void SetRegisterState(const std::vector<RegisterNameValuePair>& register_diff);
        unsigned int ExecuteInstruction(unsigned char instruction);
        unsigned int ExecuteInstruction(unsigned short instruction);
        unsigned int ExecuteInstruction(unsigned char instruction, unsigned short value);
        unsigned int ExecuteInstruction(unsigned char instruction, unsigned char value);
        ::testing::AssertionResult AssertRegisterAdapter(
                const std::string&,
                std::vector<RegisterNameValuePair>& register_diff) {
            return AssertRegisterState(register_diff);
        }
        ::testing::AssertionResult AssertRegisterState(const std::vector<RegisterNameValuePair>& register_diff);
        ::testing::AssertionResult AssertMemoryAdapter(
                const std::string&,
                const std::vector<MemoryAddressValuePair>& memory_diff) {
            return AssertMemoryState(memory_diff);
        }
        ::testing::AssertionResult AssertMemoryState(const std::vector<MemoryAddressValuePair>& memory_diff);
        // XXX: LoadROM and LoadAndRunROM are only implemented for 8-bit
        // instructions that take no arguments after the actual opcode!!!
        void LoadROM(const std::vector<TestROM>& test_rom);
        void Run(int instruction_number_to_run);
        void LoadAndRunROM(const std::vector<TestROM>& test_rom);
        unsigned short instruction_ptr() { return parser_->cpu_.rPC; }

    protected:
        TestHarness(backend::opcode_executor::OpcodeExecutor* parser) : parser_(parser) {
          // TODO(Brendan): This is pretty janky; we should have a better way of
          // mocking out the parser's functionality.
          // Turns off the internal ROM.
          parser_->memory_mapper_->Write(0xff50, 1);
        }

        virtual void SetUp() {}

        virtual void TearDown() { ClearParser(); }

        // TODO(Brendan): TestHarnesses will want to reuse the parser. Do not
        // delete it when done.
        backend::opcode_executor::OpcodeExecutor* parser_;

    private:
        bool VerifyCorrectInstruction(const std::vector<unsigned char>& instruction);
        ::testing::AssertionResult ValidateRegister(const RegisterNameValuePair& register_diff);
        void ClearParser();
        bool SetInitialState(const DiffState& initial_state);
        bool SetRegisterState(const RegisterNameValuePair& register_diff);
        void SetMemoryState(const MemoryAddressValuePair& memory_diff);
        bool LoadROM(const std::vector<InstructionExpectedStatePair>& instructions);
};

#define EXPECT_REGISTER(...)\
{\
    std::vector<test_harness::RegisterNameValuePair> reg_list_macro = __VA_ARGS__;\
    EXPECT_PRED_FORMAT1(AssertRegisterAdapter, reg_list_macro);\
}

#define EXPECT_MEMORY(...)\
{\
    std::vector<test_harness::MemoryAddressValuePair> mem_list_macro = __VA_ARGS__;\
    EXPECT_PRED_FORMAT1(AssertMemoryAdapter, mem_list_macro);\
}

} // namespace test_harness

#endif // TURBO_SANTA_TEST_HARNESS_TEST_HARNESS_H_
