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
  std::vector<InstructionFactory> list;
  list.push_back(instr(LD, 0x06, val(B), val(BIT_8)));
  list.push_back(instr(LD, 0x0e, val(C), val(BIT_8)));
  list.push_back(instr(LD, 0x16, val(D), val(BIT_8)));
  list.push_back(instr(LD, 0x1e, val(E), val(BIT_8)));
  list.push_back(instr(LD, 0x26, val(H), val(BIT_8)));
  list.push_back(instr(LD, 0x2e, val(L), val(BIT_8)));
  list.push_back(instr(LD, 0x7f, val(A), val(A)));
  list.push_back(instr(LD, 0x78, val(A), val(B)));
  list.push_back(instr(LD, 0x79, val(A), val(C)));
  list.push_back(instr(LD, 0x7a, val(A), val(D)));
  list.push_back(instr(LD, 0x7b, val(A), val(E)));
  list.push_back(instr(LD, 0x7c, val(A), val(H)));
  list.push_back(instr(LD, 0x7d, val(A), val(L)));
  list.push_back(instr(LD, 0x40, val(B), val(B)));
  list.push_back(instr(LD, 0x41, val(B), val(C)));
  list.push_back(instr(LD, 0x42, val(B), val(D)));
  list.push_back(instr(LD, 0x43, val(B), val(E)));
  list.push_back(instr(LD, 0x44, val(B), val(H)));
  list.push_back(instr(LD, 0x45, val(B), val(L)));
  list.push_back(instr(LD, 0x48, val(C), val(B)));
  list.push_back(instr(LD, 0x49, val(C), val(C)));
  list.push_back(instr(LD, 0x4a, val(C), val(D)));
  list.push_back(instr(LD, 0x4b, val(C), val(E)));
  list.push_back(instr(LD, 0x4c, val(C), val(H)));
  list.push_back(instr(LD, 0x4d, val(C), val(L)));
  list.push_back(instr(LD, 0x50, val(D), val(B)));
  list.push_back(instr(LD, 0x51, val(D), val(C)));
  list.push_back(instr(LD, 0x52, val(D), val(D)));
  list.push_back(instr(LD, 0x53, val(D), val(E)));
  list.push_back(instr(LD, 0x54, val(D), val(H)));
  list.push_back(instr(LD, 0x55, val(D), val(L)));
  list.push_back(instr(LD, 0x58, val(E), val(B)));
  list.push_back(instr(LD, 0x59, val(E), val(C)));
  list.push_back(instr(LD, 0x5a, val(E), val(D)));
  list.push_back(instr(LD, 0x5b, val(E), val(E)));
  list.push_back(instr(LD, 0x5c, val(E), val(H)));
  list.push_back(instr(LD, 0x5d, val(E), val(L)));
  list.push_back(instr(LD, 0x60, val(H), val(B)));
  list.push_back(instr(LD, 0x61, val(H), val(C)));
  list.push_back(instr(LD, 0x62, val(H), val(D)));
  list.push_back(instr(LD, 0x63, val(H), val(E)));
  list.push_back(instr(LD, 0x64, val(H), val(H)));
  list.push_back(instr(LD, 0x65, val(H), val(L)));
  list.push_back(instr(LD, 0x68, val(L), val(B)));
  list.push_back(instr(LD, 0x69, val(L), val(C)));
  list.push_back(instr(LD, 0x6a, val(L), val(D)));
  list.push_back(instr(LD, 0x6b, val(L), val(E)));
  list.push_back(instr(LD, 0x6c, val(L), val(H)));
  list.push_back(instr(LD, 0x6d, val(L), val(L)));
  list.push_back(instr(LD, 0x46, val(B), ptr(HL)));
  list.push_back(instr(LD, 0x4e, val(C), ptr(HL)));
  list.push_back(instr(LD, 0x56, val(D), ptr(HL)));
  list.push_back(instr(LD, 0x5e, val(E), ptr(HL)));
  list.push_back(instr(LD, 0x66, val(H), ptr(HL)));
  list.push_back(instr(LD, 0x6e, val(L), ptr(HL)));
  list.push_back(instr(LD, 0x70, ptr(HL), val(B)));
  list.push_back(instr(LD, 0x71, ptr(HL), val(C)));
  list.push_back(instr(LD, 0x72, ptr(HL), val(D)));
  list.push_back(instr(LD, 0x73, ptr(HL), val(E)));
  list.push_back(instr(LD, 0x74, ptr(HL), val(H)));
  list.push_back(instr(LD, 0x75, ptr(HL), val(L)));
  list.push_back(instr(LD, 0x36, ptr(HL), val(BIT_8)));
  list.push_back(instr(LD, 0x0a, val(A), ptr(BC)));
  list.push_back(instr(LD, 0x1a, val(A), ptr(DE)));
  list.push_back(instr(LD, 0x7e, val(A), ptr(HL)));
  list.push_back(instr(LD, 0xfa, val(A), ptr(BIT_16)));
  list.push_back(instr(LD, 0x3e, val(A), val(BIT_8)));
  list.push_back(instr(LD, 0x47, val(B), val(A)));
  list.push_back(instr(LD, 0x4F, val(C), val(A)));
  list.push_back(instr(LD, 0x57, val(D), val(A)));
  list.push_back(instr(LD, 0x5F, val(E), val(A)));
  list.push_back(instr(LD, 0x67, val(H), val(A)));
  list.push_back(instr(LD, 0x6F, val(L), val(A)));
  list.push_back(instr(LD, 0x02, ptr(BC), val(A)));
  list.push_back(instr(LD, 0x12, ptr(DE), val(A)));
  list.push_back(instr(LD, 0x77, ptr(HL), val(A)));
  list.push_back(instr(LD, 0xea, ptr(BIT_16), val(A)));
  list.push_back(instr(LD, 0xf2, val(A), ptr(C))); // Requires special render function
  list.push_back(instr(LD, 0xe2, ptr(C), val(A))); // Requires special render function

  list.push_back(instr(LDD, 0x3a, val(A), ptr(HL)));
  list.push_back(instr(LDD, 0x32, ptr(HL), val(A)));
  list.push_back(instr(LDI, 0x2a, val(A), ptr(HL)));
  list.push_back(instr(LDI, 0x22, ptr(HL), val(A)));

  list.push_back(instr(LD, 0xe0, ptr(BIT_8), val(A))); // Requires special render function
  list.push_back(instr(LD, 0xf0, val(A), ptr(BIT_8))); // Requires special render function
  list.push_back(instr(LD, 0x01, val(BC), val(BIT_16)));
  list.push_back(instr(LD, 0x11, val(DE), val(BIT_16)));
  list.push_back(instr(LD, 0x21, val(HL), val(BIT_16)));
  list.push_back(instr(LD, 0x31, val(SP), val(BIT_16)));
  list.push_back(instr(LD, 0xf9, val(SP), val(HL)));

  list.push_back(instr(LDHL, 0xf8, val(SP), val(BIT_8)));

  list.push_back(instr(LD, 0x08, ptr(BIT_16), val(SP)));

  list.push_back(instr(PUSH, 0xf5, val(AF)));
  list.push_back(instr(PUSH, 0xc5, val(BC)));
  list.push_back(instr(PUSH, 0xd5, val(DE)));
  list.push_back(instr(PUSH, 0xe5, val(HL)));

  list.push_back(instr(POP, 0xf1, val(AF)));
  list.push_back(instr(POP, 0xc1, val(BC)));
  list.push_back(instr(POP, 0xd1, val(DE)));
  list.push_back(instr(POP, 0xe1, val(HL)));

  list.push_back(instr(ADD, 0x87, val(A), val(A)));
  list.push_back(instr(ADD, 0x80, val(A), val(B)));
  list.push_back(instr(ADD, 0x81, val(A), val(C)));
  list.push_back(instr(ADD, 0x82, val(A), val(D)));
  list.push_back(instr(ADD, 0x83, val(A), val(E)));
  list.push_back(instr(ADD, 0x84, val(A), val(H)));
  list.push_back(instr(ADD, 0x85, val(A), val(L)));
  list.push_back(instr(ADD, 0x86, val(A), ptr(HL)));
  list.push_back(instr(ADD, 0xc6, val(A), val(BIT_8)));

  list.push_back(instr(ADC, 0x8f, val(A), val(A)));
  list.push_back(instr(ADC, 0x88, val(A), val(B)));
  list.push_back(instr(ADC, 0x89, val(A), val(C)));
  list.push_back(instr(ADC, 0x8a, val(A), val(D)));
  list.push_back(instr(ADC, 0x8b, val(A), val(E)));
  list.push_back(instr(ADC, 0x8c, val(A), val(H)));
  list.push_back(instr(ADC, 0x8d, val(A), val(L)));
  list.push_back(instr(ADC, 0x8e, val(A), ptr(HL)));
  list.push_back(instr(ADC, 0xce, val(A), val(BIT_8)));

  list.push_back(instr(SUB, 0x97, val(A), val(A)));
  list.push_back(instr(SUB, 0x90, val(A), val(B)));
  list.push_back(instr(SUB, 0x91, val(A), val(C)));
  list.push_back(instr(SUB, 0x92, val(A), val(D)));
  list.push_back(instr(SUB, 0x93, val(A), val(E)));
  list.push_back(instr(SUB, 0x94, val(A), val(H)));
  list.push_back(instr(SUB, 0x95, val(A), val(L)));
  list.push_back(instr(SUB, 0x96, val(A), ptr(HL)));
  list.push_back(instr(SUB, 0xd6, val(A), val(BIT_8)));

  list.push_back(instr(SBC, 0x9f, val(A), val(A)));
  list.push_back(instr(SBC, 0x98, val(A), val(B)));
  list.push_back(instr(SBC, 0x99, val(A), val(C)));
  list.push_back(instr(SBC, 0x9a, val(A), val(D)));
  list.push_back(instr(SBC, 0x9b, val(A), val(E)));
  list.push_back(instr(SBC, 0x9c, val(A), val(H)));
  list.push_back(instr(SBC, 0x9d, val(A), val(L)));
  list.push_back(instr(SBC, 0x9e, val(A), ptr(HL)));
  list.push_back(instr(SBC, 0xde, val(A), val(BIT_8))); // Should check opcode.

  list.push_back(instr(AND, 0xa7, val(A), val(A)));
  list.push_back(instr(AND, 0xa0, val(A), val(B)));
  list.push_back(instr(AND, 0xa1, val(A), val(C)));
  list.push_back(instr(AND, 0xa2, val(A), val(D)));
  list.push_back(instr(AND, 0xa3, val(A), val(E)));
  list.push_back(instr(AND, 0xa4, val(A), val(H)));
  list.push_back(instr(AND, 0xa5, val(A), val(L)));
  list.push_back(instr(AND, 0xa6, val(A), ptr(HL)));
  list.push_back(instr(AND, 0xe6, val(A), val(BIT_8)));

  list.push_back(instr(OR, 0xb7, val(A), val(A)));
  list.push_back(instr(OR, 0xb0, val(A), val(B)));
  list.push_back(instr(OR, 0xb1, val(A), val(C)));
  list.push_back(instr(OR, 0xb2, val(A), val(D)));
  list.push_back(instr(OR, 0xb3, val(A), val(E)));
  list.push_back(instr(OR, 0xb4, val(A), val(H)));
  list.push_back(instr(OR, 0xb5, val(A), val(L)));
  list.push_back(instr(OR, 0xb6, val(A), ptr(HL)));
  list.push_back(instr(OR, 0xf6, val(A), val(BIT_8)));

  list.push_back(instr(XOR, 0xaf, val(A), val(A)));
  list.push_back(instr(XOR, 0xa8, val(A), val(B)));
  list.push_back(instr(XOR, 0xa9, val(A), val(C)));
  list.push_back(instr(XOR, 0xaa, val(A), val(D)));
  list.push_back(instr(XOR, 0xab, val(A), val(E)));
  list.push_back(instr(XOR, 0xac, val(A), val(H)));
  list.push_back(instr(XOR, 0xad, val(A), val(L)));
  list.push_back(instr(XOR, 0xae, val(A), ptr(HL)));
  list.push_back(instr(XOR, 0xee, val(A), val(BIT_8)));

  list.push_back(instr(CP, 0xbf, val(A), val(A)));
  list.push_back(instr(CP, 0xb8, val(A), val(B)));
  list.push_back(instr(CP, 0xb9, val(A), val(C)));
  list.push_back(instr(CP, 0xba, val(A), val(D)));
  list.push_back(instr(CP, 0xbb, val(A), val(E)));
  list.push_back(instr(CP, 0xbc, val(A), val(H)));
  list.push_back(instr(CP, 0xbd, val(A), val(L)));
  list.push_back(instr(CP, 0xbe, val(A), ptr(HL)));
  list.push_back(instr(CP, 0xfe, val(A), val(BIT_8)));

  list.push_back(instr(INC, 0x3c, val(A)));
  list.push_back(instr(INC, 0x04, val(B)));
  list.push_back(instr(INC, 0x0c, val(C)));
  list.push_back(instr(INC, 0x14, val(D)));
  list.push_back(instr(INC, 0x1c, val(E)));
  list.push_back(instr(INC, 0x24, val(H)));
  list.push_back(instr(INC, 0x2c, val(L)));
  list.push_back(instr(INC, 0x34, ptr(HL)));

  list.push_back(instr(DEC, 0x3d, val(A)));
  list.push_back(instr(DEC, 0x05, val(B)));
  list.push_back(instr(DEC, 0x0d, val(C)));
  list.push_back(instr(DEC, 0x15, val(D)));
  list.push_back(instr(DEC, 0x1d, val(E)));
  list.push_back(instr(DEC, 0x25, val(H)));
  list.push_back(instr(DEC, 0x2d, val(L)));
  list.push_back(instr(DEC, 0x35, ptr(HL)));

  list.push_back(instr(ADD, 0x09, val(HL), val(BC)));
  list.push_back(instr(ADD, 0x19, val(HL), val(DE)));
  list.push_back(instr(ADD, 0x29, val(HL), val(HL)));
  list.push_back(instr(ADD, 0x39, val(HL), val(SP)));

  list.push_back(instr(ADD, 0xe8, val(SP), val(BIT_8))); // Signed.

  list.push_back(instr(INC, 0x03, val(BC)));
  list.push_back(instr(INC, 0x13, val(DE)));
  list.push_back(instr(INC, 0x23, val(HL)));
  list.push_back(instr(INC, 0x33, val(SP)));

  list.push_back(instr(DEC, 0x0b, val(BC)));
  list.push_back(instr(DEC, 0x1b, val(DE)));
  list.push_back(instr(DEC, 0x2b, val(HL)));
  list.push_back(instr(DEC, 0x3b, val(SP)));

  list.push_back(instr(SWAP, 0xcb37, val(A), BIT_16));
  list.push_back(instr(SWAP, 0xcb30, val(B), BIT_16));
  list.push_back(instr(SWAP, 0xcb31, val(C), BIT_16));
  list.push_back(instr(SWAP, 0xcb32, val(D), BIT_16));
  list.push_back(instr(SWAP, 0xcb33, val(E), BIT_16));
  list.push_back(instr(SWAP, 0xcb34, val(H), BIT_16));
  list.push_back(instr(SWAP, 0xcb35, val(L), BIT_16));
  list.push_back(instr(SWAP, 0xcb36, ptr(HL), BIT_16));

  list.push_back(instr(DAA, 0x27));
  list.push_back(instr(CPL, 0x2f));
  list.push_back(instr(CCF, 0x3f));
  list.push_back(instr(SCF, 0x37));
  list.push_back(instr(NOP, 0x00));
  list.push_back(instr(HALT, 0x76));
  list.push_back(instr(STOP, 0x1000, BIT_16));
  list.push_back(instr(DI, 0xf3));
  list.push_back(instr(EI, 0xfb));

  list.push_back(instr(RLCA, 0x07));
  list.push_back(instr(RLA, 0x17));
  list.push_back(instr(RRCA, 0x0f));
  list.push_back(instr(RRA, 0x1f));

  list.push_back(instr(RLC, 0xcb07, val(A), BIT_16));
  list.push_back(instr(RLC, 0xcb00, val(B), BIT_16));
  list.push_back(instr(RLC, 0xcb01, val(C), BIT_16));
  list.push_back(instr(RLC, 0xcb02, val(D), BIT_16));
  list.push_back(instr(RLC, 0xcb03, val(E), BIT_16));
  list.push_back(instr(RLC, 0xcb04, val(H), BIT_16));
  list.push_back(instr(RLC, 0xcb05, val(L), BIT_16));
  list.push_back(instr(RLC, 0xcb06, ptr(HL), BIT_16));

  list.push_back(instr(RL, 0xcb17, val(A), BIT_16));
  list.push_back(instr(RL, 0xcb10, val(B), BIT_16));
  list.push_back(instr(RL, 0xcb11, val(C), BIT_16));
  list.push_back(instr(RL, 0xcb12, val(D), BIT_16));
  list.push_back(instr(RL, 0xcb13, val(E), BIT_16));
  list.push_back(instr(RL, 0xcb14, val(H), BIT_16));
  list.push_back(instr(RL, 0xcb15, val(L), BIT_16));
  list.push_back(instr(RL, 0xcb16, ptr(HL), BIT_16));

  list.push_back(instr(RRC, 0xcb0f, val(A), BIT_16));
  list.push_back(instr(RRC, 0xcb08, val(B), BIT_16));
  list.push_back(instr(RRC, 0xcb09, val(C), BIT_16));
  list.push_back(instr(RRC, 0xcb0a, val(D), BIT_16));
  list.push_back(instr(RRC, 0xcb0b, val(E), BIT_16));
  list.push_back(instr(RRC, 0xcb0c, val(H), BIT_16));
  list.push_back(instr(RRC, 0xcb0d, val(L), BIT_16));
  list.push_back(instr(RRC, 0xcb0e, ptr(HL), BIT_16));

  list.push_back(instr(RR, 0xcb1f, val(A), BIT_16));
  list.push_back(instr(RR, 0xcb18, val(B), BIT_16));
  list.push_back(instr(RR, 0xcb19, val(C), BIT_16));
  list.push_back(instr(RR, 0xcb1a, val(D), BIT_16));
  list.push_back(instr(RR, 0xcb1b, val(E), BIT_16));
  list.push_back(instr(RR, 0xcb1c, val(H), BIT_16));
  list.push_back(instr(RR, 0xcb1d, val(L), BIT_16));
  list.push_back(instr(RR, 0xcb1e, ptr(HL), BIT_16));

  list.push_back(instr(SLA, 0xcb27, val(A), BIT_16));
  list.push_back(instr(SLA, 0xcb20, val(B), BIT_16));
  list.push_back(instr(SLA, 0xcb21, val(C), BIT_16));
  list.push_back(instr(SLA, 0xcb22, val(D), BIT_16));
  list.push_back(instr(SLA, 0xcb23, val(E), BIT_16));
  list.push_back(instr(SLA, 0xcb24, val(H), BIT_16));
  list.push_back(instr(SLA, 0xcb25, val(L), BIT_16));
  list.push_back(instr(SLA, 0xcb26, ptr(HL), BIT_16));

  list.push_back(instr(SRA, 0xcb2f, val(A), BIT_16));
  list.push_back(instr(SRA, 0xcb28, val(B), BIT_16));
  list.push_back(instr(SRA, 0xcb29, val(C), BIT_16));
  list.push_back(instr(SRA, 0xcb2a, val(D), BIT_16));
  list.push_back(instr(SRA, 0xcb2b, val(E), BIT_16));
  list.push_back(instr(SRA, 0xcb2c, val(H), BIT_16));
  list.push_back(instr(SRA, 0xcb2d, val(L), BIT_16));
  list.push_back(instr(SRA, 0xcb2e, ptr(HL), BIT_16));

  list.push_back(instr(SRL, 0xcb3f, val(A), BIT_16));
  list.push_back(instr(SRL, 0xcb38, val(B), BIT_16));
  list.push_back(instr(SRL, 0xcb39, val(C), BIT_16));
  list.push_back(instr(SRL, 0xcb3a, val(D), BIT_16));
  list.push_back(instr(SRL, 0xcb3b, val(E), BIT_16));
  list.push_back(instr(SRL, 0xcb3c, val(H), BIT_16));
  list.push_back(instr(SRL, 0xcb3d, val(L), BIT_16));
  list.push_back(instr(SRL, 0xcb3e, ptr(HL), BIT_16));

  list.push_back(instr(BIT, 0xcb47, bit(), val(A), BIT_16));
  list.push_back(instr(BIT, 0xcb40, bit(), val(B), BIT_16));
  list.push_back(instr(BIT, 0xcb41, bit(), val(C), BIT_16));
  list.push_back(instr(BIT, 0xcb42, bit(), val(D), BIT_16));
  list.push_back(instr(BIT, 0xcb43, bit(), val(E), BIT_16));
  list.push_back(instr(BIT, 0xcb44, bit(), val(H), BIT_16));
  list.push_back(instr(BIT, 0xcb45, bit(), val(L), BIT_16));
  list.push_back(instr(BIT, 0xcb46, bit(), ptr(HL), BIT_16));

  list.push_back(instr(SET, 0xcbc7, bit(), val(A), BIT_16));
  list.push_back(instr(SET, 0xcbc0, bit(), val(B), BIT_16));
  list.push_back(instr(SET, 0xcbc1, bit(), val(C), BIT_16));
  list.push_back(instr(SET, 0xcbc2, bit(), val(D), BIT_16));
  list.push_back(instr(SET, 0xcbc3, bit(), val(E), BIT_16));
  list.push_back(instr(SET, 0xcbc4, bit(), val(H), BIT_16));
  list.push_back(instr(SET, 0xcbc5, bit(), val(L), BIT_16));
  list.push_back(instr(SET, 0xcbc6, bit(), ptr(HL), BIT_16));

  list.push_back(instr(RES, 0xcb87, bit(), val(A), BIT_16));
  list.push_back(instr(RES, 0xcb80, bit(), val(B), BIT_16));
  list.push_back(instr(RES, 0xcb81, bit(), val(C), BIT_16));
  list.push_back(instr(RES, 0xcb82, bit(), val(D), BIT_16));
  list.push_back(instr(RES, 0xcb83, bit(), val(E), BIT_16));
  list.push_back(instr(RES, 0xcb84, bit(), val(H), BIT_16));
  list.push_back(instr(RES, 0xcb85, bit(), val(L), BIT_16));
  list.push_back(instr(RES, 0xcb86, bit(), ptr(HL), BIT_16));

  list.push_back(jump(JP, 0xc3, val(BIT_16)));

  list.push_back(jump(JP, 0xc2, val(ZN), val(BIT_16)));
  list.push_back(jump(JP, 0xca, val(ZF), val(BIT_16)));
  list.push_back(jump(JP, 0xd2, val(CN), val(BIT_16)));
  list.push_back(jump(JP, 0xda, val(CF), val(BIT_16)));

  list.push_back(jump(JP, 0xe9, ptr(HL)));

  list.push_back(jump(JR, 0x18, val(BIT_8)));

  list.push_back(jump(JR, 0x20, val(ZN), val(BIT_8)));
  list.push_back(jump(JR, 0x28, val(ZF), val(BIT_8)));
  list.push_back(jump(JR, 0x30, val(CN), val(BIT_8)));
  list.push_back(jump(JR, 0x38, val(CF), val(BIT_8)));

  list.push_back(jump(CALL, 0xcd, val(BIT_16)));

  list.push_back(jump(CALL, 0xc4, val(ZN), val(BIT_16)));
  list.push_back(jump(CALL, 0xcc, val(ZF), val(BIT_16)));
  list.push_back(jump(CALL, 0xd4, val(CN), val(BIT_16)));
  list.push_back(jump(CALL, 0xdc, val(CF), val(BIT_16)));

  list.push_back(jump(RST, 0xc7, con(0x00)));
  list.push_back(jump(RST, 0xcf, con(0x08)));
  list.push_back(jump(RST, 0xd7, con(0x10)));
  list.push_back(jump(RST, 0xdf, con(0x18)));
  list.push_back(jump(RST, 0xe7, con(0x20)));
  list.push_back(jump(RST, 0xef, con(0x28)));
  list.push_back(jump(RST, 0xf7, con(0x30)));
  list.push_back(jump(RST, 0xff, con(0x38)));

  list.push_back(jump(RET, 0xc9));

  list.push_back(jump(RET, 0xc0, val(ZN)));
  list.push_back(jump(RET, 0xc8, val(ZF)));
  list.push_back(jump(RET, 0xd0, val(CN)));
  list.push_back(jump(RET, 0xd8, val(CF)));

  list.push_back(jump(RETI, 0xd9));
  return list;
}
} // namespace

std::map<uint16_t, InstructionFactory> CreateInstructionMap() {
  typedef std::vector<InstructionFactory>::iterator Iterator;
  std::vector<InstructionFactory> factory_list = CreateFactoryList();
  std::map<uint16_t, InstructionFactory> map;
  for (std::move_iterator<Iterator> iter(factory_list.begin());
       iter != std::move_iterator<Iterator>(factory_list.end()); iter++) {
    auto map_iter = map.find(iter->instruction());
    if (map_iter == map.end()) {
      // NOTE: This makes the vector returned from CreateFactoryList invalid!!!
      map[iter->instruction()] = *iter;
    } else {
      LOG(FATAL) << "Opcode, 0x" << std::hex << iter->instruction()
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

std::map<Register, std::string> CreateRegisterMap() {
  return {
    {A, "A"},
    {B, "B"},
    {C, "C"},
    {D, "D"},
    {E, "E"},
    {H, "H"},
    {L, "L"},
    {AF, "AF"},
    {BC, "BC"},
    {DE, "DE"},
    {HL, "HL"},
    {PC, "PC"},
    {SP, "SP"},
    {ZF, "Z"},
    {NF, "N"},
    {HF, "H"},
    {CF, "C"},
    {ZN, "NZ"},
    {NN, "NN"},
    {HN, "NH"},
    {CN, "NC"},
  };
}

} // namespace decompiler
} // namespace decompiler
