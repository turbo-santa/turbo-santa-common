#ifndef TURBO_SANTA_TEST_HARNESS_TEST_HARNESS_H_
#define TURBO_SANTA_TEST_HARNESS_TEST_HARNESS_H_

#include <memory>
#include <vector>
#include "back_end/opcode_parser.h"

namespace test_harness {

using back_end::opcode_parser::OpcodeParser;

struct RegisterNameValuePair {
    enum RegisterName {
        B,
        C,
        D,
        E,
        H,
        L,
        A,
        BC,
        DE,
        HL,
        AF,
        PC,
        SP,
        FZ,
        FN,
        FH,
        FC
    };

    RegisterName register_name;
    // If the register is 8 bit it does not use the top byte.
    unsigned short register_value;
};

struct MemoryAddressValuePair {
    unsigned short address;
    unsigned char value;
};

struct DiffState {
    std::vector<RegisterNameValuePair> registers;
    std::vector<MemoryAddressValuePair> memory;
};

struct InstructionExpectedStatePair {
    std::vector<unsigned char> instruction;
    DiffState expected_state;
};

class TestHarness {
    public:
        TestHarness() : parser_(new OpcodeParser(nullptr, 0)) {}

        bool RunTest(const DiffState& initial_state, const std::vector<InstructionExpectedStatePair>& instructions);
    private:
        bool ExecuteAndValidateInstruction(const InstructionExpectedStatePair& instruction);
        bool VerifyCorrectInstruction(const std::vector<unsigned char>& instruction);
        bool ValidateRegister(const RegisterNameValuePair& register_diff);
        bool ClearParser();
        bool SetInitialState(const DiffState& initial_state);
        bool SetRegisterState(const RegisterNameValuePair& register_diff);
        bool LoadROM(const std::vector<InstructionExpectedStatePair>& instructions);
        std::unique_ptr<OpcodeParser> parser_;
};

} // namespace test_harness

#endif // TURBO_SANTA_TEST_HARNESS_TEST_HARNESS_H_
