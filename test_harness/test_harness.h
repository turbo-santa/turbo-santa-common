#ifndef TURBO_SANTA_TEST_HARNESS_TEST_HARNESS_H_
#define TURBO_SANTA_TEST_HARNESS_TEST_HARNESS_H_

#include <memory>
#include <vector>
#include "back_end/opcode_parser.h"
#include "test_harness/test_harness_utils.h"

namespace test_harness {

class TestHarness {
    public:
        TestHarness() : parser_(new back_end::opcode_parser::OpcodeParser(nullptr, 0)) {}

        bool RunTest(const DiffState& initial_state, const std::vector<InstructionExpectedStatePair>& instructions);
    private:
        bool ExecuteAndValidateInstruction(const InstructionExpectedStatePair& instruction);
        bool VerifyCorrectInstruction(const std::vector<unsigned char>& instruction);
        bool ValidateRegister(const RegisterNameValuePair& register_diff);
        bool ClearParser();
        bool SetInitialState(const DiffState& initial_state);
        bool SetRegisterState(const RegisterNameValuePair& register_diff);
        bool LoadROM(const std::vector<InstructionExpectedStatePair>& instructions);
        std::unique_ptr<back_end::opcode_parser::OpcodeParser> parser_;
};

} // namespace test_harness

#endif // TURBO_SANTA_TEST_HARNESS_TEST_HARNESS_H_
