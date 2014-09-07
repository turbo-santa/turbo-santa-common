#include "back_end/opcode_parser.h"

using back_end::opcode_parser::OpcodeParser;

int main() {
    unsigned char test_input[] = { 
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00
    };

    OpcodeParser parser(test_input);
    return 0;
}
