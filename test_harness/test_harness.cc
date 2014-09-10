#include "test_harness/test_harness.h"

#include <memory>
#include "back_end/opcode_handlers.h"
#include "back_end/registers.h"

namespace test_harness {
    using std::unique_ptr;
    using std::vector;
    using back_end::handlers::mem_map;
    using back_end::registers::cpu;

    bool TestHarness::RunTest(const DiffState& initial_state, const vector<InstructionExpectedStatePair>& instructions) {
        ClearParser(); // Zero out all values of the parser.
        SetInitialState(initial_state); // Set the initial state.
        LoadROM(instructions); // Set the value of the space of memory.

        // Execute instruction.
        for (const InstructionExpectedStatePair& instruction : instructions) {
            // TODO(Brendan): Return a descriptive error message instead of a bool.
            if (!ExecuteAndValidateInstruction(instruction)) {
                return false;
            }
        }
        return true;
    }

    // TODO(Brendan): Implement rest of method.
    bool TestHarness::ExecuteAndValidateInstruction(const InstructionExpectedStatePair& instruction) {
        VerifyCorrectInstruction(instruction.instruction);
        parser_->ReadInstruction();

        for (const RegisterNameValuePair& register_diff : instruction.expected_state.registers) {
            if (!ValidateRegister(register_diff)) {
                return false;
            }
        }

        for (const MemoryAddressValuePair& state_diff : instruction.expected_state.memory) {
            if (mem_map->get_pointer()[state_diff.address] != state_diff.value) {
                return false;
            }
        }
        return true;
    }

    bool TestHarness::ValidateRegister(const RegisterNameValuePair& register_diff) {
        unsigned short value = register_diff.register_value;
        switch (register_diff.register_name) {
            case RegisterNameValuePair::B:
                return value == cpu.bc_struct.rB;
            case RegisterNameValuePair::C:
                return value == cpu.bc_struct.rC;
            case RegisterNameValuePair::D:
                return value == cpu.de_struct.rD;
            case RegisterNameValuePair::E:
                return value == cpu.de_struct.rE;
            case RegisterNameValuePair::H:
                return value == cpu.hl_struct.rH;
            case RegisterNameValuePair::L:
                return value == cpu.hl_struct.rL;
            case RegisterNameValuePair::A:
                return value == cpu.flag_struct.rA;
            case RegisterNameValuePair::BC:
                return value == cpu.rBC;
            case RegisterNameValuePair::DE:
                return value == cpu.rDE;
            case RegisterNameValuePair::HL:
                return value == cpu.rHL;
            case RegisterNameValuePair::AF:
                return value == cpu.rAF;
            case RegisterNameValuePair::PC:
                return value == cpu.rPC;
            case RegisterNameValuePair::SP:
                return value == cpu.rSP;
            case RegisterNameValuePair::FZ:
                return value == cpu.flag_struct.rF.Z;
            case RegisterNameValuePair::FN:
                return value == cpu.flag_struct.rF.N;
            case RegisterNameValuePair::FH:
                return value == cpu.flag_struct.rF.H;
            case RegisterNameValuePair::FC:
                return value == cpu.flag_struct.rF.C;
        }
        return true;
    }

    bool TestHarness::VerifyCorrectInstruction(const vector<unsigned char>& instruction) {
        for (unsigned long i = 0; i < instruction.size(); i++) {
            if (parser_->rom_[i] != instruction[i]) {
                return false;
            }
        }
        return true;
    }

    bool TestHarness::ClearParser() {
        for (int i = 0; i < 0x10000; i++) {
            mem_map->get_pointer()[i] = 0;
        }
        return true;
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
