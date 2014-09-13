#ifndef TURBO_SANTA_TEST_HARNESS_TEST_HARNESS_H_
#define TURBO_SANTA_TEST_HARNESS_TEST_HARNESS_H_

#include <memory>
#include <vector>
#include "back_end/opcode_parser.h"
#include "test_harness/test_harness_utils.h"

#include "third_party/gtest/include/gtest/gtest.h"

namespace test_harness {

class TestHarness : public ::testing::Test {
    public:
        void SetRegisterState(const std::vector<RegisterNameValuePair>& register_diff);
        void ExecuteInstruction(unsigned char instruction);
        // void ExecuteInstruction(unsigned short instruction);
        void AssertRegisterState(const std::vector<RegisterNameValuePair>& register_diff);
        unsigned char* get_rom_ptr() { return parser_->rom_; }
        int get_instruction_ptr() { return parser_->instruction_ptr_; }

    protected:
        TestHarness(back_end::opcode_parser::OpcodeParser* parser) : parser_(parser) {}
        virtual void TearDown() { ClearParser(); }

    private:
        bool VerifyCorrectInstruction(const std::vector<unsigned char>& instruction);
        void ValidateRegister(const RegisterNameValuePair& register_diff);
        void ClearParser();
        bool SetInitialState(const DiffState& initial_state);
        bool SetRegisterState(const RegisterNameValuePair& register_diff);
        bool LoadROM(const std::vector<InstructionExpectedStatePair>& instructions);
        // TODO(Brendan): TestHarnesses will want to reuse the parser. Do not
        // delete it when done.
        back_end::opcode_parser::OpcodeParser* parser_;
};

} // namespace test_harness

#endif // TURBO_SANTA_TEST_HARNESS_TEST_HARNESS_H_
