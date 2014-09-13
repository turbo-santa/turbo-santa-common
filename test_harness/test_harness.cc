#include "test_harness/test_harness.h"

#include <memory>
#include <string>
#include "back_end/opcode_handlers.h"
#include "back_end/registers.h"

namespace test_harness {
using std::string;
using std::unique_ptr;
using std::vector;
using back_end::handlers::mem_map;
using back_end::registers::cpu;

namespace {
void ExpectRegisterEquals(unsigned short expected, unsigned short actual, const string& name) {
    EXPECT_EQ(expected, actual) << "Expected register " << name << " to be " << expected << " but was " << actual;
}
} // namespace

void TestHarness::SetRegisterState(const vector<RegisterNameValuePair>& register_diff_list) {
    for (const RegisterNameValuePair& register_diff : register_diff_list) {
        SetRegisterState(register_diff);
    }
}

void TestHarness::AssertRegisterState(const vector<RegisterNameValuePair>& register_diff_list) {
    for (const RegisterNameValuePair& register_diff : register_diff_list) {
        ValidateRegister(register_diff);
    }
}

void TestHarness::ExecuteInstruction(unsigned char instruction) {
    parser_->rom_[parser_->instruction_ptr_] = instruction; // We put the instruction in right before it gets called.
    parser_->ReadInstruction();
}

void TestHarness::ValidateRegister(const RegisterNameValuePair& register_diff) {
    unsigned short value = register_diff.register_value;
    switch (register_diff.register_name) {
        case RegisterNameValuePair::B:
            ExpectRegisterEquals(value, cpu.bc_struct.rB, "B");
            break;
        case RegisterNameValuePair::C:
            ExpectRegisterEquals(value, cpu.bc_struct.rC, "C");
            break;
        case RegisterNameValuePair::D:
            ExpectRegisterEquals(value, cpu.de_struct.rD, "D");
            break;
        case RegisterNameValuePair::E:
            ExpectRegisterEquals(value, cpu.de_struct.rE, "E");
            break;
        case RegisterNameValuePair::H:
            ExpectRegisterEquals(value, cpu.hl_struct.rH, "H");
            break;
        case RegisterNameValuePair::L:
            ExpectRegisterEquals(value, cpu.hl_struct.rL, "L");
            break;
        case RegisterNameValuePair::A:
            ExpectRegisterEquals(value, cpu.flag_struct.rA, "A");
            break;
        case RegisterNameValuePair::BC:
            ExpectRegisterEquals(value, cpu.rBC, "BC");
            break;
        case RegisterNameValuePair::DE:
            ExpectRegisterEquals(value, cpu.rDE, "DE");
            break;
        case RegisterNameValuePair::HL:
            ExpectRegisterEquals(value, cpu.rHL, "HL");
            break;
        case RegisterNameValuePair::AF:
            ExpectRegisterEquals(value, cpu.rAF, "AF");
            break;
        case RegisterNameValuePair::PC:
            ExpectRegisterEquals(value, cpu.rPC, "PC");
            break;
        case RegisterNameValuePair::SP:
            ExpectRegisterEquals(value, cpu.rSP, "SP");
            break;
        case RegisterNameValuePair::FZ:
            ExpectRegisterEquals(value, cpu.flag_struct.rF.Z, "FZ");
            break;
        case RegisterNameValuePair::FN:
            ExpectRegisterEquals(value, cpu.flag_struct.rF.N, "FN");
            break;
        case RegisterNameValuePair::FH:
            ExpectRegisterEquals(value, cpu.flag_struct.rF.H, "FH");
            break;
        case RegisterNameValuePair::FC:
            ExpectRegisterEquals(value, cpu.flag_struct.rF.C, "FC");
            break;
    }
}

bool TestHarness::VerifyCorrectInstruction(const vector<unsigned char>& instruction) {
    for (unsigned long i = 0; i < instruction.size(); i++) {
        if (parser_->rom_[i] != instruction[i]) {
            return false;
        }
    }
    return true;
}

void TestHarness::ClearParser() {
    parser_->instruction_ptr_ = 0;
    for (int i = 0; i < 0x10000; i++) {
        mem_map->get_pointer()[i] = 0;
    }
    typedef RegisterNameValuePair::RegisterName R;
    SetRegisterState({
            {R::B, 0},
            {R::C, 0},
            {R::D, 0},
            {R::E, 0},
            {R::H, 0},
            {R::L, 0},
            {R::A, 0},
            {R::BC, 0},
            {R::DE, 0},
            {R::HL, 0},
            {R::AF, 0},
            {R::PC, 0},
            {R::SP, 0},
            {R::FZ, 0},
            {R::FC, 0},
            {R::FH, 0},
            {R::FN, 0}});
}

bool TestHarness::SetInitialState(const DiffState& state_diff) {
    for (unsigned long i = 0; i < state_diff.memory.size(); i++) {
        mem_map->get_pointer()[state_diff.memory[i].address] = state_diff.memory[i].value;
    }

    for (const RegisterNameValuePair& register_diff : state_diff.registers) {
        if (!SetRegisterState(register_diff)) {
            return false;
        }
    }
    return true;
}

bool TestHarness::SetRegisterState(const RegisterNameValuePair& state_diff) {
    unsigned short value = state_diff.register_value;
    switch (state_diff.register_name) {
        case RegisterNameValuePair::B:
            cpu.bc_struct.rB = value;
            return true;
        case RegisterNameValuePair::C:
            cpu.bc_struct.rC = value;
            return true;
        case RegisterNameValuePair::D:
            cpu.de_struct.rD = value;
            return true;
        case RegisterNameValuePair::E:
            cpu.de_struct.rE = value;
            return true;
        case RegisterNameValuePair::H:
            cpu.hl_struct.rH = value;
            return true;
        case RegisterNameValuePair::L:
            cpu.hl_struct.rL = value;
            return true;
        case RegisterNameValuePair::A:
            cpu.flag_struct.rA = value;
            return true;
        case RegisterNameValuePair::BC:
            cpu.rBC = value;
            return true;
        case RegisterNameValuePair::DE:
            cpu.rDE = value;
            return true;
        case RegisterNameValuePair::HL:
            cpu.rHL = value;
            return true;
        case RegisterNameValuePair::AF:
            cpu.rAF = value;
            return true;
        case RegisterNameValuePair::PC:
            cpu.rPC = value;
            return true;
        case RegisterNameValuePair::SP:
            cpu.rSP = value;
            return true;
        case RegisterNameValuePair::FZ:
            cpu.flag_struct.rF.Z = value;
            return true;
        case RegisterNameValuePair::FN:
            cpu.flag_struct.rF.N = value;
            return true;
        case RegisterNameValuePair::FH:
            cpu.flag_struct.rF.H = value;
            return true;
        case RegisterNameValuePair::FC:
            cpu.flag_struct.rF.C = value;
            return true;
    }
    return true;
}

// TODO(Brendan): Add the opcodes in the correct place in memory.
bool TestHarness::LoadROM(const vector<InstructionExpectedStatePair>& instructions) {
    int i = 0;
    for (const InstructionExpectedStatePair& instruction : instructions) {
        for (unsigned char byte : instruction.instruction) {
            mem_map->get_pointer()[i] = byte;
            i++;
        }
    }
    return true;
}
} // namespace test_harness
