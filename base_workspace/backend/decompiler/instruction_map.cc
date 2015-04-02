#include "backend/decompiler/instruction_map.h"

#include <vector>
#include "submodules/glog/src/glog/logging.h"

namespace back_end {
namespace decompiler {

namespace {
static const Register A = Register::A;
static const Register B = Register::B;
static const Register C = Register::C;
static const Register D = Register::D;
static const Register E = Register::E;
static const Register H = Register::H;
static const Register L = Register::L;
static const Register AF = Register::AF;
static const Register BC = Register::BC;
static const Register DE = Register::DE;
static const Register HL = Register::HL;
static const Register SP = Register::SP;
static const Register PC = Register::PC;
static const Register ZF = Register::ZF;
static const Register NF = Register::NF;
static const Register HF = Register::HF;
static const Register CF = Register::CF;
static const Register ZN = Register::ZN;
static const Register NN = Register::NN;
static const Register HN = Register::HN;
static const Register CN = Register::CN;

static const ValueWidth BIT_0 = ValueWidth::BIT_0;
static const ValueWidth BIT_8 = ValueWidth::BIT_8;
static const ValueWidth BIT_16 = ValueWidth::BIT_16;
static const ValueWidth BIT_24 = ValueWidth::BIT_24;

static const Opcode ADC = Opcode::ADC;
static const Opcode ADD = Opcode::ADD;
static const Opcode AND = Opcode::AND;
static const Opcode BIT = Opcode::BIT;
static const Opcode CALL = Opcode::CALL;
static const Opcode CCF = Opcode::CCF;
static const Opcode CP = Opcode::CP;
static const Opcode CPL = Opcode::CPL;
static const Opcode DAA = Opcode::DAA;
static const Opcode DEC = Opcode::DEC;
static const Opcode DI = Opcode::DI;
static const Opcode EI = Opcode::EI;
static const Opcode HALT = Opcode::HALT;
static const Opcode INC = Opcode::INC;
static const Opcode JP = Opcode::JP;
static const Opcode JR = Opcode::JR;
static const Opcode LD = Opcode::LD;
static const Opcode LDD = Opcode::LDD;
static const Opcode LDHL = Opcode::LDHL;
static const Opcode LDI = Opcode::LDI;
static const Opcode NOP = Opcode::NOP;
static const Opcode OR = Opcode::OR;
static const Opcode POP = Opcode::POP;
static const Opcode PUSH = Opcode::PUSH;
static const Opcode RES = Opcode::RES;
static const Opcode RET = Opcode::RET;
static const Opcode RETI = Opcode::RETI;
static const Opcode RL = Opcode::RL;
static const Opcode RLA = Opcode::RLA;
static const Opcode RLC = Opcode::RLC;
static const Opcode RLCA = Opcode::RLCA;
static const Opcode RR = Opcode::RR;
static const Opcode RRA = Opcode::RRA;
static const Opcode RRC = Opcode::RRC;
static const Opcode RRCA = Opcode::RRCA;
static const Opcode RST = Opcode::RST;
static const Opcode SBC = Opcode::SBC;
static const Opcode SCF = Opcode::SCF;
static const Opcode SET = Opcode::SET;
static const Opcode SLA = Opcode::SLA;
static const Opcode SRA = Opcode::SRA;
static const Opcode SRL = Opcode::SRL;
static const Opcode STOP = Opcode::STOP;
static const Opcode SUB = Opcode::SUB;
static const Opcode SWAP = Opcode::SWAP;
static const Opcode XOR = Opcode::XOR;

std::vector<InstructionFactory> CreateFactoryList() {
  return {
    instr(LD, 0x06, val(B), val(BIT_8)),
    instr(LD, 0x0e, val(C), val(BIT_8)),
    instr(LD, 0x16, val(D), val(BIT_8)),
    instr(LD, 0x1e, val(E), val(BIT_8)),
    instr(LD, 0x26, val(H), val(BIT_8)),
    instr(LD, 0x2e, val(L), val(BIT_8)),
    instr(LD, 0x7f, val(A), val(A)),
    instr(LD, 0x78, val(A), val(B)),
    instr(LD, 0x79, val(A), val(C)),
    instr(LD, 0x7a, val(A), val(D)),
    instr(LD, 0x7b, val(A), val(E)),
    instr(LD, 0x7c, val(A), val(H)),
    instr(LD, 0x7d, val(A), val(L)),
    instr(LD, 0x40, val(B), val(B)),
    instr(LD, 0x41, val(B), val(C)),
    instr(LD, 0x42, val(B), val(D)),
    instr(LD, 0x43, val(B), val(E)),
    instr(LD, 0x44, val(B), val(H)),
    instr(LD, 0x45, val(B), val(L)),
    instr(LD, 0x48, val(C), val(B)),
    instr(LD, 0x49, val(C), val(C)),
    instr(LD, 0x4a, val(C), val(D)),
    instr(LD, 0x4b, val(C), val(E)),
    instr(LD, 0x4c, val(C), val(H)),
    instr(LD, 0x4d, val(C), val(L)),
    instr(LD, 0x50, val(D), val(B)),
    instr(LD, 0x51, val(D), val(C)),
    instr(LD, 0x52, val(D), val(D)),
    instr(LD, 0x53, val(D), val(E)),
    instr(LD, 0x54, val(D), val(H)),
    instr(LD, 0x55, val(D), val(L)),
    instr(LD, 0x58, val(E), val(B)),
    instr(LD, 0x59, val(E), val(C)),
    instr(LD, 0x5a, val(E), val(D)),
    instr(LD, 0x5b, val(E), val(E)),
    instr(LD, 0x5c, val(E), val(H)),
    instr(LD, 0x5d, val(E), val(L)),
    instr(LD, 0x60, val(H), val(B)),
    instr(LD, 0x61, val(H), val(C)),
    instr(LD, 0x62, val(H), val(D)),
    instr(LD, 0x63, val(H), val(E)),
    instr(LD, 0x64, val(H), val(H)),
    instr(LD, 0x65, val(H), val(L)),
    instr(LD, 0x68, val(L), val(B)),
    instr(LD, 0x69, val(L), val(C)),
    instr(LD, 0x6a, val(L), val(D)),
    instr(LD, 0x6b, val(L), val(E)),
    instr(LD, 0x6c, val(L), val(H)),
    instr(LD, 0x6d, val(L), val(L)),
    instr(LD, 0x7e, val(A), ptr(HL)),
    instr(LD, 0x46, val(B), ptr(HL)),
    instr(LD, 0x4e, val(C), ptr(HL)),
    instr(LD, 0x56, val(D), ptr(HL)),
    instr(LD, 0x5e, val(E), ptr(HL)),
    instr(LD, 0x66, val(H), ptr(HL)),
    instr(LD, 0x6e, val(L), ptr(HL)),
    instr(LD, 0x70, ptr(HL), val(B)),
    instr(LD, 0x71, ptr(HL), val(C)),
    instr(LD, 0x72, ptr(HL), val(D)),
    instr(LD, 0x73, ptr(HL), val(E)),
    instr(LD, 0x74, ptr(HL), val(H)),
    instr(LD, 0x75, ptr(HL), val(L)),
    instr(LD, 0x36, ptr(HL), val(BIT_8)),
    instr(LD, 0x0a, val(A), ptr(BC)),
    instr(LD, 0x1a, val(A), ptr(DE)),
    instr(LD, 0x7e, val(A), ptr(HL)),
    instr(LD, 0xfa, val(A), ptr(BIT_16)),
    instr(LD, 0x3e, val(A), val(BIT_8)),
    instr(LD, 0x47, val(B), val(A)),
    instr(LD, 0x4F, val(C), val(A)),
    instr(LD, 0x57, val(D), val(A)),
    instr(LD, 0x5F, val(E), val(A)),
    instr(LD, 0x67, val(H), val(A)),
    instr(LD, 0x6F, val(L), val(A)),
    instr(LD, 0x02, ptr(BC), val(A)),
    instr(LD, 0x12, ptr(DE), val(A)),
    instr(LD, 0x77, ptr(HL), val(A)),
    instr(LD, 0xea, ptr(BIT_16), val(A)),
    instr(LD, 0xf2, val(A), ptr(C)), // Requires special render function
    instr(LD, 0xe2, ptr(C), val(A)), // Requires special render function

    instr(LDD, 0x3a, val(A), ptr(HL)),
    instr(LDD, 0x32, ptr(HL), val(A)),
    instr(LDI, 0x2a, val(A), ptr(HL)),
    instr(LDI, 0x22, ptr(HL), val(A)),

    instr(LD, 0xe0, ptr(BIT_8), val(A)), // Requires special render function
    instr(LD, 0xf0, val(A), ptr(BIT_8)), // Requires special render function
    instr(LD, 0x01, val(BC), val(BIT_8)),
    instr(LD, 0x11, val(DE), val(BIT_8)),
    instr(LD, 0x21, val(HL), val(BIT_8)),
    instr(LD, 0x31, val(SP), val(BIT_8)),
    instr(LD, 0xf9, val(SP), val(HL)),

    instr(LDHL, 0xf8, val(SP), val(BIT_8)),

    instr(LD, 0x08, ptr(BIT_16), val(SP)),

    instr(PUSH, 0xf5, val(AF)),
    instr(PUSH, 0xc5, val(BC)),
    instr(PUSH, 0xd5, val(DE)),
    instr(PUSH, 0xe5, val(HL)),

    instr(POP, 0xf1, val(AF)),
    instr(POP, 0xc1, val(BC)),
    instr(POP, 0xd1, val(DE)),
    instr(POP, 0xe1, val(HL)),

    instr(ADD, 0x87, val(A), val(A)),
    instr(ADD, 0x80, val(A), val(B)),
    instr(ADD, 0x81, val(A), val(C)),
    instr(ADD, 0x82, val(A), val(D)),
    instr(ADD, 0x83, val(A), val(E)),
    instr(ADD, 0x84, val(A), val(H)),
    instr(ADD, 0x85, val(A), val(L)),
    instr(ADD, 0x86, val(A), ptr(HL)),
    instr(ADD, 0xc6, val(A), val(BIT_8)),

    instr(ADC, 0x8f, val(A), val(A)),
    instr(ADC, 0x88, val(A), val(B)),
    instr(ADC, 0x89, val(A), val(C)),
    instr(ADC, 0x8a, val(A), val(D)),
    instr(ADC, 0x8b, val(A), val(E)),
    instr(ADC, 0x8c, val(A), val(H)),
    instr(ADC, 0x8d, val(A), val(L)),
    instr(ADC, 0x8e, val(A), ptr(HL)),
    instr(ADC, 0xce, val(A), val(BIT_8)),

    instr(SUB, 0x97, val(A), val(A)),
    instr(SUB, 0x90, val(A), val(B)),
    instr(SUB, 0x91, val(A), val(C)),
    instr(SUB, 0x92, val(A), val(D)),
    instr(SUB, 0x93, val(A), val(E)),
    instr(SUB, 0x94, val(A), val(H)),
    instr(SUB, 0x95, val(A), val(L)),
    instr(SUB, 0x96, val(A), ptr(HL)),
    instr(SUB, 0xd6, val(A), val(BIT_8)),

    instr(SBC, 0x9f, val(A), val(A)),
    instr(SBC, 0x98, val(A), val(B)),
    instr(SBC, 0x99, val(A), val(C)),
    instr(SBC, 0x9a, val(A), val(D)),
    instr(SBC, 0x9b, val(A), val(E)),
    instr(SBC, 0x9c, val(A), val(H)),
    instr(SBC, 0x9d, val(A), val(L)),
    instr(SBC, 0x9e, val(A), ptr(HL)),
    instr(SBC, 0xde, val(A), val(BIT_8)), // Should check opcode.

    instr(AND, 0xa7, val(A), val(A)),
    instr(AND, 0xa0, val(A), val(B)),
    instr(AND, 0xa1, val(A), val(C)),
    instr(AND, 0xa2, val(A), val(D)),
    instr(AND, 0xa3, val(A), val(E)),
    instr(AND, 0xa4, val(A), val(H)),
    instr(AND, 0xa5, val(A), val(L)),
    instr(AND, 0xa6, val(A), ptr(HL)),
    instr(AND, 0xe6, val(A), val(BIT_8)),

    instr(OR, 0xb7, val(A), val(A)),
    instr(OR, 0xb0, val(A), val(B)),
    instr(OR, 0xb1, val(A), val(C)),
    instr(OR, 0xb2, val(A), val(D)),
    instr(OR, 0xb3, val(A), val(E)),
    instr(OR, 0xb4, val(A), val(H)),
    instr(OR, 0xb5, val(A), val(L)),
    instr(OR, 0xb6, val(A), ptr(HL)),
    instr(OR, 0xf6, val(A), val(BIT_8)),

    instr(XOR, 0xaf, val(A), val(A)),
    instr(XOR, 0xa8, val(A), val(B)),
    instr(XOR, 0xa9, val(A), val(C)),
    instr(XOR, 0xaa, val(A), val(D)),
    instr(XOR, 0xab, val(A), val(E)),
    instr(XOR, 0xac, val(A), val(H)),
    instr(XOR, 0xad, val(A), val(L)),
    instr(XOR, 0xae, val(A), ptr(HL)),
    instr(XOR, 0xee, val(A), val(BIT_8)),

    instr(CP, 0xbf, val(A), val(A)),
    instr(CP, 0xb8, val(A), val(B)),
    instr(CP, 0xb9, val(A), val(C)),
    instr(CP, 0xba, val(A), val(D)),
    instr(CP, 0xbb, val(A), val(E)),
    instr(CP, 0xbc, val(A), val(H)),
    instr(CP, 0xbd, val(A), val(L)),
    instr(CP, 0xbe, val(A), ptr(HL)),
    instr(CP, 0xfe, val(A), val(BIT_8)),

    instr(INC, 0x3c, val(A)),
    instr(INC, 0x04, val(B)),
    instr(INC, 0x0c, val(C)),
    instr(INC, 0x14, val(D)),
    instr(INC, 0x1c, val(E)),
    instr(INC, 0x24, val(H)),
    instr(INC, 0x2c, val(L)),
    instr(INC, 0x34, ptr(HL)),

    instr(DEC, 0x3d, val(A)),
    instr(DEC, 0x05, val(B)),
    instr(DEC, 0x0d, val(C)),
    instr(DEC, 0x15, val(D)),
    instr(DEC, 0x1d, val(E)),
    instr(DEC, 0x25, val(H)),
    instr(DEC, 0x2d, val(L)),
    instr(DEC, 0x35, ptr(HL)),

    instr(ADD, 0x09, val(HL), val(BC)),
    instr(ADD, 0x19, val(HL), val(DE)),
    instr(ADD, 0x29, val(HL), val(HL)),
    instr(ADD, 0x39, val(HL), val(SP)),

    instr(ADD, 0xe8, val(SP), val(BIT_8)), // Signed.
    
    instr(INC, 0x03, val(BC)),
    instr(INC, 0x13, val(DE)),
    instr(INC, 0x23, val(HL)),
    instr(INC, 0x33, val(SP)),

    instr(DEC, 0x0b, val(BC)),
    instr(DEC, 0x1b, val(DE)),
    instr(DEC, 0x2b, val(HL)),
    instr(DEC, 0x3b, val(SP)),

    instr(SWAP, 0xcb37, val(A), BIT_16),
    instr(SWAP, 0xcb30, val(B), BIT_16),
    instr(SWAP, 0xcb31, val(C), BIT_16),
    instr(SWAP, 0xcb32, val(D), BIT_16),
    instr(SWAP, 0xcb33, val(E), BIT_16),
    instr(SWAP, 0xcb34, val(H), BIT_16),
    instr(SWAP, 0xcb35, val(L), BIT_16),
    instr(SWAP, 0xcb36, ptr(HL), BIT_16),

    instr(DAA, 0x27),
    instr(CPL, 0x2f),
    instr(CCF, 0x3f),
    instr(SCF, 0x37),
    instr(NOP, 0x00),
    instr(HALT, 0x76),
    instr(STOP, 0x1000, BIT_16),
    instr(DI, 0xf3),
    instr(EI, 0xfb),

    instr(RLCA, 0x07),
    instr(RLA, 0x17),
    instr(RRCA, 0x0f),
    instr(RRA, 0x1f),

    instr(RLC, 0xcb07, val(A), BIT_16),
    instr(RLC, 0xcb00, val(B), BIT_16),
    instr(RLC, 0xcb01, val(C), BIT_16),
    instr(RLC, 0xcb02, val(D), BIT_16),
    instr(RLC, 0xcb03, val(E), BIT_16),
    instr(RLC, 0xcb04, val(H), BIT_16),
    instr(RLC, 0xcb05, val(L), BIT_16),
    instr(RLC, 0xcb06, ptr(HL), BIT_16),

    instr(RL, 0xcb17, val(A), BIT_16),
    instr(RL, 0xcb10, val(B), BIT_16),
    instr(RL, 0xcb11, val(C), BIT_16),
    instr(RL, 0xcb12, val(D), BIT_16),
    instr(RL, 0xcb13, val(E), BIT_16),
    instr(RL, 0xcb14, val(H), BIT_16),
    instr(RL, 0xcb15, val(L), BIT_16),
    instr(RL, 0xcb16, ptr(HL), BIT_16),

    instr(RRC, 0xcb0f, val(A), BIT_16),
    instr(RRC, 0xcb08, val(B), BIT_16),
    instr(RRC, 0xcb09, val(C), BIT_16),
    instr(RRC, 0xcb0a, val(D), BIT_16),
    instr(RRC, 0xcb0b, val(E), BIT_16),
    instr(RRC, 0xcb0c, val(H), BIT_16),
    instr(RRC, 0xcb0d, val(L), BIT_16),
    instr(RRC, 0xcb0e, ptr(HL), BIT_16),

    instr(RR, 0xcb1f, val(A), BIT_16),
    instr(RR, 0xcb18, val(B), BIT_16),
    instr(RR, 0xcb19, val(C), BIT_16),
    instr(RR, 0xcb1a, val(D), BIT_16),
    instr(RR, 0xcb1b, val(E), BIT_16),
    instr(RR, 0xcb1c, val(H), BIT_16),
    instr(RR, 0xcb1d, val(L), BIT_16),
    instr(RR, 0xcb1e, ptr(HL), BIT_16),

    instr(SLA, 0xcb27, val(A), BIT_16),
    instr(SLA, 0xcb20, val(B), BIT_16),
    instr(SLA, 0xcb21, val(C), BIT_16),
    instr(SLA, 0xcb22, val(D), BIT_16),
    instr(SLA, 0xcb23, val(E), BIT_16),
    instr(SLA, 0xcb24, val(H), BIT_16),
    instr(SLA, 0xcb25, val(L), BIT_16),
    instr(SLA, 0xcb26, ptr(HL), BIT_16),

    instr(SRA, 0xcb2f, val(A), BIT_16),
    instr(SRA, 0xcb28, val(B), BIT_16),
    instr(SRA, 0xcb29, val(C), BIT_16),
    instr(SRA, 0xcb2a, val(D), BIT_16),
    instr(SRA, 0xcb2b, val(E), BIT_16),
    instr(SRA, 0xcb2c, val(H), BIT_16),
    instr(SRA, 0xcb2d, val(L), BIT_16),
    instr(SRA, 0xcb2e, ptr(HL), BIT_16),

    instr(SRL, 0xcb3f, val(A), BIT_16),
    instr(SRL, 0xcb38, val(B), BIT_16),
    instr(SRL, 0xcb39, val(C), BIT_16),
    instr(SRL, 0xcb3a, val(D), BIT_16),
    instr(SRL, 0xcb3b, val(E), BIT_16),
    instr(SRL, 0xcb3c, val(H), BIT_16),
    instr(SRL, 0xcb3d, val(L), BIT_16),
    instr(SRL, 0xcb3e, ptr(HL), BIT_16),

    instr(BIT, 0xcb47, bit(), val(A), BIT_16),
    instr(BIT, 0xcb40, bit(), val(B), BIT_16),
    instr(BIT, 0xcb41, bit(), val(C), BIT_16),
    instr(BIT, 0xcb42, bit(), val(D), BIT_16),
    instr(BIT, 0xcb43, bit(), val(E), BIT_16),
    instr(BIT, 0xcb44, bit(), val(H), BIT_16),
    instr(BIT, 0xcb45, bit(), val(L), BIT_16),
    instr(BIT, 0xcb46, bit(), ptr(HL), BIT_16),

    instr(SET, 0xcbc7, bit(), val(A), BIT_16),
    instr(SET, 0xcbc0, bit(), val(B), BIT_16),
    instr(SET, 0xcbc1, bit(), val(C), BIT_16),
    instr(SET, 0xcbc2, bit(), val(D), BIT_16),
    instr(SET, 0xcbc3, bit(), val(E), BIT_16),
    instr(SET, 0xcbc4, bit(), val(H), BIT_16),
    instr(SET, 0xcbc5, bit(), val(L), BIT_16),
    instr(SET, 0xcbc6, bit(), ptr(HL), BIT_16),

    instr(RES, 0xcb87, bit(), val(A), BIT_16),
    instr(RES, 0xcb80, bit(), val(B), BIT_16),
    instr(RES, 0xcb81, bit(), val(C), BIT_16),
    instr(RES, 0xcb82, bit(), val(D), BIT_16),
    instr(RES, 0xcb83, bit(), val(E), BIT_16),
    instr(RES, 0xcb84, bit(), val(H), BIT_16),
    instr(RES, 0xcb85, bit(), val(L), BIT_16),
    instr(RES, 0xcb86, bit(), ptr(HL), BIT_16),

    jump(JP, 0xc3, val(BIT_16)),

    jump(JP, 0xc2, val(ZN), val(BIT_16)),
    jump(JP, 0xca, val(ZF), val(BIT_16)),
    jump(JP, 0xd2, val(CN), val(BIT_16)),
    jump(JP, 0xda, val(CF), val(BIT_16)),

    jump(JP, 0xe9, ptr(HL)),

    jump(JR, 0x18, val(BIT_8)),

    jump(JR, 0x20, val(ZN), val(BIT_8)),
    jump(JR, 0x28, val(ZF), val(BIT_8)),
    jump(JR, 0x30, val(CN), val(BIT_8)),
    jump(JR, 0x38, val(CF), val(BIT_8)),

    jump(CALL, 0xcd, val(BIT_16)),

    jump(CALL, 0xc4, val(ZN), val(BIT_16)),
    jump(CALL, 0xcc, val(ZF), val(BIT_16)),
    jump(CALL, 0xd4, val(CN), val(BIT_16)),
    jump(CALL, 0xdc, val(CF), val(BIT_16)),

    jump(RST, 0xc7, con(0x00)),
    jump(RST, 0xcf, con(0x08)),
    jump(RST, 0xd7, con(0x10)),
    jump(RST, 0xdf, con(0x18)),
    jump(RST, 0xe7, con(0x20)),
    jump(RST, 0xef, con(0x28)),
    jump(RST, 0xf7, con(0x30)),
    jump(RST, 0xff, con(0x38)),

    jump(RET, 0xc9),

    jump(RET, 0xc0, val(ZN)),
    jump(RET, 0xc8, val(ZF)),
    jump(RET, 0xd0, val(CN)),
    jump(RET, 0xd8, val(CF)),

    jump(RETI, 0xd9)
  };
}
} // namespace

std::map<uint16_t, InstructionFactory> CreateInstructionMap() {
  std::map<uint16_t, InstructionFactory> map;
  for (const auto& factory : CreateFactoryList()) {
    auto iter = map.find(factory.instruction());
    if (iter == map.end()) {
      map[factory.instruction()] = factory;
    } else {
      LOG(FATAL) << "Opcode, 0x" << std::hex << factory.instruction()
          << ", was repeated in instruction map.";
    }
  }
  return map;
}

std::map<Opcode, std::string> CreateNameMap() {
  return {
  {ADC,  "ADC" },
  {ADD,  "ADD" },
  {AND,  "AND" },
  {BIT,  "BIT" },
  {CALL, "CALL"},
  {CCF,  "CCF" },
  {CP,   "CP"  },
  {CPL,  "CPL" },
  {DAA,  "DAA" },
  {DEC,  "DEC" },
  {DI,   "DI"  },
  {EI,   "EI"  },
  {HALT, "HALT"},
  {INC,  "INC" },
  {JP,   "JP"  },
  {JR,   "JR"  },
  {LD,   "LD"  },
  {LDD,  "LDD" },
  {LDHL, "LDHL"},
  {LDI,  "LDI" },
  {NOP,  "NOP" },
  {OR,   "OR"  },
  {POP,  "POP" },
  {PUSH, "PUSH"},
  {RES,  "RES" },
  {RET,  "RET" },
  {RETI, "RETI"},
  {RL,   "RL"  },
  {RLA,  "RLA" },
  {RLC,  "RLC" },
  {RLCA, "RLCA"},
  {RR,   "RR"  },
  {RRA,  "RRA" },
  {RRC,  "RRC" },
  {RRCA, "RRCA"},
  {RST,  "RST" },
  {SBC,  "SBC" },
  {SCF,  "SCF" },
  {SET,  "SET" },
  {SLA,  "SLA" },
  {SRA,  "SRA" },
  {SRL,  "SRL" },
  {STOP, "STOP"},
  {SUB,  "SUB" },
  {SWAP, "SWAP"},
  {XOR,  "XOR" },
  };
}

} // namespace decompiler
} // namespace decompiler
