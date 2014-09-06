#include "back_end/opcode_handlers.h"

namespace back_end {
namespace handlers {

int Add8Bit(unsigned char* rom, int instruction_ptr, int opcode_length) {
}

int Dec8Bit(unsigned char* rom, int instruction_ptr, int opcode_length) {
    ALU8BitRegisterRegister* opcode = static_cast<ALU8BitRegisterRegister*>(rom + instruction_ptr);
}

} // namespace handlers
} // namespace back_end
