#include <iostream>
#include "test_harness/test_harness.h"

using std::cout;
using std::endl;
using test_harness::TestHarness;

int main() {
    TestHarness harness;
    cout << harness.RunTest({{}, {}}, {
            {{0x00}, {{}, {}}},
            {{0x00}, {{}, {}}},
            {{0x00}, {{}, {}}},
            {{0x00}, {{}, {}}},
            }) << endl;
    return 0;
}
