#ifndef TURBO_SANTA_TEST_HARNESS_TEST_HARNESS_UTILS_H_
#define TURBO_SANTA_TEST_HARNESS_TEST_HARNESS_UTILS_H_

#include <vector>

namespace test_harness {

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

} // namespace test_harness

#endif // TURBO_SANTA_TEST_HARNESS_TEST_HARNESS_UTILS_H_
