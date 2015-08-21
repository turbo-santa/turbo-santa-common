#include "cc/backend/decompiler/instruction_map.h"

#include <vector>
#include "instruction_factory_helpers.h"
#include "glog/logging.h"

namespace backend {
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
  list.push_back(instr(LD, 0x06, 8, val(B), val(BIT_8)));
  list.push_back(instr(LD, 0x0e, 8, val(C), val(BIT_8)));
  list.push_back(instr(LD, 0x16, 8, val(D), val(BIT_8)));
  list.push_back(instr(LD, 0x1e, 8, val(E), val(BIT_8)));
  list.push_back(instr(LD, 0x26, 8, val(H), val(BIT_8)));
  list.push_back(instr(LD, 0x2e, 8, val(L), val(BIT_8)));
  list.push_back(instr(LD, 0x7f, 4, val(A), val(A)));
  list.push_back(instr(LD, 0x78, 4, val(A), val(B)));
  list.push_back(instr(LD, 0x79, 4, val(A), val(C)));
  list.push_back(instr(LD, 0x7a, 4, val(A), val(D)));
  list.push_back(instr(LD, 0x7b, 4, val(A), val(E)));
  list.push_back(instr(LD, 0x7c, 4, val(A), val(H)));
  list.push_back(instr(LD, 0x7d, 4, val(A), val(L)));
  list.push_back(instr(LD, 0x40, 4, val(B), val(B)));
  list.push_back(instr(LD, 0x41, 4, val(B), val(C)));
  list.push_back(instr(LD, 0x42, 4, val(B), val(D)));
  list.push_back(instr(LD, 0x43, 4, val(B), val(E)));
  list.push_back(instr(LD, 0x44, 4, val(B), val(H)));
  list.push_back(instr(LD, 0x45, 4, val(B), val(L)));
  list.push_back(instr(LD, 0x48, 4, val(C), val(B)));
  list.push_back(instr(LD, 0x49, 4, val(C), val(C)));
  list.push_back(instr(LD, 0x4a, 4, val(C), val(D)));
  list.push_back(instr(LD, 0x4b, 4, val(C), val(E)));
  list.push_back(instr(LD, 0x4c, 4, val(C), val(H)));
  list.push_back(instr(LD, 0x4d, 4, val(C), val(L)));
  list.push_back(instr(LD, 0x50, 4, val(D), val(B)));
  list.push_back(instr(LD, 0x51, 4, val(D), val(C)));
  list.push_back(instr(LD, 0x52, 4, val(D), val(D)));
  list.push_back(instr(LD, 0x53, 4, val(D), val(E)));
  list.push_back(instr(LD, 0x54, 4, val(D), val(H)));
  list.push_back(instr(LD, 0x55, 4, val(D), val(L)));
  list.push_back(instr(LD, 0x58, 4, val(E), val(B)));
  list.push_back(instr(LD, 0x59, 4, val(E), val(C)));
  list.push_back(instr(LD, 0x5a, 4, val(E), val(D)));
  list.push_back(instr(LD, 0x5b, 4, val(E), val(E)));
  list.push_back(instr(LD, 0x5c, 4, val(E), val(H)));
  list.push_back(instr(LD, 0x5d, 4, val(E), val(L)));
  list.push_back(instr(LD, 0x60, 4, val(H), val(B)));
  list.push_back(instr(LD, 0x61, 4, val(H), val(C)));
  list.push_back(instr(LD, 0x62, 4, val(H), val(D)));
  list.push_back(instr(LD, 0x63, 4, val(H), val(E)));
  list.push_back(instr(LD, 0x64, 4, val(H), val(H)));
  list.push_back(instr(LD, 0x65, 4, val(H), val(L)));
  list.push_back(instr(LD, 0x68, 4, val(L), val(B)));
  list.push_back(instr(LD, 0x69, 4, val(L), val(C)));
  list.push_back(instr(LD, 0x6a, 4, val(L), val(D)));
  list.push_back(instr(LD, 0x6b, 4, val(L), val(E)));
  list.push_back(instr(LD, 0x6c, 4, val(L), val(H)));
  list.push_back(instr(LD, 0x6d, 4, val(L), val(L)));
  list.push_back(instr(LD, 0x46, 8, val(B), ptr(HL)));
  list.push_back(instr(LD, 0x4e, 8, val(C), ptr(HL)));
  list.push_back(instr(LD, 0x56, 8, val(D), ptr(HL)));
  list.push_back(instr(LD, 0x5e, 8, val(E), ptr(HL)));
  list.push_back(instr(LD, 0x66, 8, val(H), ptr(HL)));
  list.push_back(instr(LD, 0x6e, 8, val(L), ptr(HL)));
  list.push_back(instr(LD, 0x70, 8, ptr(HL), val(B)));
  list.push_back(instr(LD, 0x71, 8, ptr(HL), val(C)));
  list.push_back(instr(LD, 0x72, 8, ptr(HL), val(D)));
  list.push_back(instr(LD, 0x73, 8, ptr(HL), val(E)));
  list.push_back(instr(LD, 0x74, 8, ptr(HL), val(H)));
  list.push_back(instr(LD, 0x75, 8, ptr(HL), val(L)));
  list.push_back(instr(LD, 0x36, 12, ptr(HL), val(BIT_8)));
  list.push_back(instr(LD, 0x0a, 8, val(A), ptr(BC)));
  list.push_back(instr(LD, 0x1a, 8, val(A), ptr(DE)));
  list.push_back(instr(LD, 0x7e, 8, val(A), ptr(HL)));
  list.push_back(instr(LD, 0xfa, 16, val(A), ptr(BIT_16)));
  list.push_back(instr(LD, 0x3e, 8, val(A), val(BIT_8)));
  list.push_back(instr(LD, 0x47, 4, val(B), val(A)));
  list.push_back(instr(LD, 0x4F, 4, val(C), val(A)));
  list.push_back(instr(LD, 0x57, 4, val(D), val(A)));
  list.push_back(instr(LD, 0x5F, 4, val(E), val(A)));
  list.push_back(instr(LD, 0x67, 4, val(H), val(A)));
  list.push_back(instr(LD, 0x6F, 4, val(L), val(A)));
  list.push_back(instr(LD, 0x02, 8, ptr(BC), val(A)));
  list.push_back(instr(LD, 0x12, 8, ptr(DE), val(A)));
  list.push_back(instr(LD, 0x77, 8, ptr(HL), val(A)));
  list.push_back(instr(LD, 0xea, 16, ptr(BIT_16), val(A)));
  list.push_back(instr(LD, 0xf2, 8, val(A), ptr(C))); // Requires special render function
  list.push_back(instr(LD, 0xe2, 8, ptr(C), val(A))); // Requires special render function

  list.push_back(instr(LDD, 0x3a, 8, val(A), ptr(HL)));
  list.push_back(instr(LDD, 0x32, 8, ptr(HL), val(A)));
  list.push_back(instr(LDI, 0x2a, 8, val(A), ptr(HL)));
  list.push_back(instr(LDI, 0x22, 8, ptr(HL), val(A)));

  list.push_back(instr(LD, 0xe0, 12, ptr(BIT_8), val(A))); // Requires special render function
  list.push_back(instr(LD, 0xf0, 12, val(A), ptr(BIT_8))); // Requires special render function
  list.push_back(instr(LD, 0x01, 12, val(BC), val(BIT_16)));
  list.push_back(instr(LD, 0x11, 12, val(DE), val(BIT_16)));
  list.push_back(instr(LD, 0x21, 12, val(HL), val(BIT_16)));
  list.push_back(instr(LD, 0x31, 12, val(SP), val(BIT_16)));
  list.push_back(instr(LD, 0xf9, 8, val(SP), val(HL)));

  list.push_back(instr(LDHL, 0xf8, 12, val(SP), val(BIT_8)));

  list.push_back(instr(LD, 0x08, 20, ptr(BIT_16), val(SP)));

  list.push_back(instr(PUSH, 0xf5, 16, val(AF)));
  list.push_back(instr(PUSH, 0xc5, 16, val(BC)));
  list.push_back(instr(PUSH, 0xd5, 16, val(DE)));
  list.push_back(instr(PUSH, 0xe5, 16, val(HL)));

  list.push_back(instr(POP, 0xf1, 12, val(AF)));
  list.push_back(instr(POP, 0xc1, 12, val(BC)));
  list.push_back(instr(POP, 0xd1, 12, val(DE)));
  list.push_back(instr(POP, 0xe1, 12, val(HL)));

  list.push_back(instr(ADD, 0x87, 4, val(A), val(A)));
  list.push_back(instr(ADD, 0x80, 4, val(A), val(B)));
  list.push_back(instr(ADD, 0x81, 4, val(A), val(C)));
  list.push_back(instr(ADD, 0x82, 4, val(A), val(D)));
  list.push_back(instr(ADD, 0x83, 4, val(A), val(E)));
  list.push_back(instr(ADD, 0x84, 4, val(A), val(H)));
  list.push_back(instr(ADD, 0x85, 4, val(A), val(L)));
  list.push_back(instr(ADD, 0x86, 8, val(A), ptr(HL)));
  list.push_back(instr(ADD, 0xc6, 8, val(A), val(BIT_8)));

  list.push_back(instr(ADC, 0x8f, 4, val(A), val(A)));
  list.push_back(instr(ADC, 0x88, 4, val(A), val(B)));
  list.push_back(instr(ADC, 0x89, 4, val(A), val(C)));
  list.push_back(instr(ADC, 0x8a, 4, val(A), val(D)));
  list.push_back(instr(ADC, 0x8b, 4, val(A), val(E)));
  list.push_back(instr(ADC, 0x8c, 4, val(A), val(H)));
  list.push_back(instr(ADC, 0x8d, 4, val(A), val(L)));
  list.push_back(instr(ADC, 0x8e, 8, val(A), ptr(HL)));
  list.push_back(instr(ADC, 0xce, 8, val(A), val(BIT_8)));

  list.push_back(instr(SUB, 0x97, 4, val(A), val(A)));
  list.push_back(instr(SUB, 0x90, 4, val(A), val(B)));
  list.push_back(instr(SUB, 0x91, 4, val(A), val(C)));
  list.push_back(instr(SUB, 0x92, 4, val(A), val(D)));
  list.push_back(instr(SUB, 0x93, 4, val(A), val(E)));
  list.push_back(instr(SUB, 0x94, 4, val(A), val(H)));
  list.push_back(instr(SUB, 0x95, 4, val(A), val(L)));
  list.push_back(instr(SUB, 0x96, 8, val(A), ptr(HL)));
  list.push_back(instr(SUB, 0xd6, 8, val(A), val(BIT_8)));

  list.push_back(instr(SBC, 0x9f, 4, val(A), val(A)));
  list.push_back(instr(SBC, 0x98, 4, val(A), val(B)));
  list.push_back(instr(SBC, 0x99, 4, val(A), val(C)));
  list.push_back(instr(SBC, 0x9a, 4, val(A), val(D)));
  list.push_back(instr(SBC, 0x9b, 4, val(A), val(E)));
  list.push_back(instr(SBC, 0x9c, 4, val(A), val(H)));
  list.push_back(instr(SBC, 0x9d, 4, val(A), val(L)));
  list.push_back(instr(SBC, 0x9e, 8, val(A), ptr(HL)));
  list.push_back(instr(SBC, 0xde, 8, val(A), val(BIT_8))); // Should check opcode.

  list.push_back(instr(AND, 0xa7, 4, val(A), val(A)));
  list.push_back(instr(AND, 0xa0, 4, val(A), val(B)));
  list.push_back(instr(AND, 0xa1, 4, val(A), val(C)));
  list.push_back(instr(AND, 0xa2, 4, val(A), val(D)));
  list.push_back(instr(AND, 0xa3, 4, val(A), val(E)));
  list.push_back(instr(AND, 0xa4, 4, val(A), val(H)));
  list.push_back(instr(AND, 0xa5, 4, val(A), val(L)));
  list.push_back(instr(AND, 0xa6, 8, val(A), ptr(HL)));
  list.push_back(instr(AND, 0xe6, 8, val(A), val(BIT_8)));

  list.push_back(instr(OR, 0xb7, 4, val(A), val(A)));
  list.push_back(instr(OR, 0xb0, 4, val(A), val(B)));
  list.push_back(instr(OR, 0xb1, 4, val(A), val(C)));
  list.push_back(instr(OR, 0xb2, 4, val(A), val(D)));
  list.push_back(instr(OR, 0xb3, 4, val(A), val(E)));
  list.push_back(instr(OR, 0xb4, 4, val(A), val(H)));
  list.push_back(instr(OR, 0xb5, 4, val(A), val(L)));
  list.push_back(instr(OR, 0xb6, 8, val(A), ptr(HL)));
  list.push_back(instr(OR, 0xf6, 8, val(A), val(BIT_8)));

  list.push_back(instr(XOR, 0xaf, 4, val(A), val(A)));
  list.push_back(instr(XOR, 0xa8, 4, val(A), val(B)));
  list.push_back(instr(XOR, 0xa9, 4, val(A), val(C)));
  list.push_back(instr(XOR, 0xaa, 4, val(A), val(D)));
  list.push_back(instr(XOR, 0xab, 4, val(A), val(E)));
  list.push_back(instr(XOR, 0xac, 4, val(A), val(H)));
  list.push_back(instr(XOR, 0xad, 4, val(A), val(L)));
  list.push_back(instr(XOR, 0xae, 8, val(A), ptr(HL)));
  list.push_back(instr(XOR, 0xee, 8, val(A), val(BIT_8)));

  list.push_back(instr(CP, 0xbf, 4, val(A), val(A)));
  list.push_back(instr(CP, 0xb8, 4, val(A), val(B)));
  list.push_back(instr(CP, 0xb9, 4, val(A), val(C)));
  list.push_back(instr(CP, 0xba, 4, val(A), val(D)));
  list.push_back(instr(CP, 0xbb, 4, val(A), val(E)));
  list.push_back(instr(CP, 0xbc, 4, val(A), val(H)));
  list.push_back(instr(CP, 0xbd, 4, val(A), val(L)));
  list.push_back(instr(CP, 0xbe, 8, val(A), ptr(HL)));
  list.push_back(instr(CP, 0xfe, 8, val(A), val(BIT_8)));

  list.push_back(instr(INC, 0x3c, 4, val(A)));
  list.push_back(instr(INC, 0x04, 4, val(B)));
  list.push_back(instr(INC, 0x0c, 4, val(C)));
  list.push_back(instr(INC, 0x14, 4, val(D)));
  list.push_back(instr(INC, 0x1c, 4, val(E)));
  list.push_back(instr(INC, 0x24, 4, val(H)));
  list.push_back(instr(INC, 0x2c, 4, val(L)));
  list.push_back(instr(INC, 0x34, 12, ptr(HL)));

  list.push_back(instr(DEC, 0x3d, 4, val(A)));
  list.push_back(instr(DEC, 0x05, 4, val(B)));
  list.push_back(instr(DEC, 0x0d, 4, val(C)));
  list.push_back(instr(DEC, 0x15, 4, val(D)));
  list.push_back(instr(DEC, 0x1d, 4, val(E)));
  list.push_back(instr(DEC, 0x25, 4, val(H)));
  list.push_back(instr(DEC, 0x2d, 4, val(L)));
  list.push_back(instr(DEC, 0x35, 12, ptr(HL)));

  list.push_back(instr(ADD, 0x09, 8, val(HL), val(BC)));
  list.push_back(instr(ADD, 0x19, 8, val(HL), val(DE)));
  list.push_back(instr(ADD, 0x29, 8, val(HL), val(HL)));
  list.push_back(instr(ADD, 0x39, 8, val(HL), val(SP)));

  list.push_back(instr(ADD, 0xe8, 16, val(SP), val(BIT_8))); // Signed.

  list.push_back(instr(INC, 0x03, 8, val(BC)));
  list.push_back(instr(INC, 0x13, 8, val(DE)));
  list.push_back(instr(INC, 0x23, 8, val(HL)));
  list.push_back(instr(INC, 0x33, 8, val(SP)));

  list.push_back(instr(DEC, 0x0b, 8, val(BC)));
  list.push_back(instr(DEC, 0x1b, 8, val(DE)));
  list.push_back(instr(DEC, 0x2b, 8, val(HL)));
  list.push_back(instr(DEC, 0x3b, 8, val(SP)));

  list.push_back(instr(SWAP, 0xcb37, 8, val(A), BIT_16));
  list.push_back(instr(SWAP, 0xcb30, 8, val(B), BIT_16));
  list.push_back(instr(SWAP, 0xcb31, 8, val(C), BIT_16));
  list.push_back(instr(SWAP, 0xcb32, 8, val(D), BIT_16));
  list.push_back(instr(SWAP, 0xcb33, 8, val(E), BIT_16));
  list.push_back(instr(SWAP, 0xcb34, 8, val(H), BIT_16));
  list.push_back(instr(SWAP, 0xcb35, 8, val(L), BIT_16));
  list.push_back(instr(SWAP, 0xcb36, 16, ptr(HL), BIT_16));

  list.push_back(instr(DAA, 0x27, 4));
  list.push_back(instr(CPL, 0x2f, 4));
  list.push_back(instr(CCF, 0x3f, 4));
  list.push_back(instr(SCF, 0x37, 4));
  list.push_back(instr(NOP, 0x00, 4));
  list.push_back(instr(HALT, 0x76, 4));
  list.push_back(instr(STOP, 0x1000, 4, BIT_16));
  list.push_back(instr(DI, 0xf3, 4));
  list.push_back(instr(EI, 0xfb, 4));

  list.push_back(instr(RLCA, 0x07, 4));
  list.push_back(instr(RLA, 0x17, 4));
  list.push_back(instr(RRCA, 0x0f, 4));
  list.push_back(instr(RRA, 0x1f, 4));

  list.push_back(instr(RLC, 0xcb07, 8, val(A), BIT_16));
  list.push_back(instr(RLC, 0xcb00, 8, val(B), BIT_16));
  list.push_back(instr(RLC, 0xcb01, 8, val(C), BIT_16));
  list.push_back(instr(RLC, 0xcb02, 8, val(D), BIT_16));
  list.push_back(instr(RLC, 0xcb03, 8, val(E), BIT_16));
  list.push_back(instr(RLC, 0xcb04, 8, val(H), BIT_16));
  list.push_back(instr(RLC, 0xcb05, 8, val(L), BIT_16));
  list.push_back(instr(RLC, 0xcb06, 16, ptr(HL), BIT_16));

  list.push_back(instr(RL, 0xcb17, 8, val(A), BIT_16));
  list.push_back(instr(RL, 0xcb10, 8, val(B), BIT_16));
  list.push_back(instr(RL, 0xcb11, 8, val(C), BIT_16));
  list.push_back(instr(RL, 0xcb12, 8, val(D), BIT_16));
  list.push_back(instr(RL, 0xcb13, 8, val(E), BIT_16));
  list.push_back(instr(RL, 0xcb14, 8, val(H), BIT_16));
  list.push_back(instr(RL, 0xcb15, 8, val(L), BIT_16));
  list.push_back(instr(RL, 0xcb16, 16, ptr(HL), BIT_16));

  list.push_back(instr(RRC, 0xcb0f, 8, val(A), BIT_16));
  list.push_back(instr(RRC, 0xcb08, 8, val(B), BIT_16));
  list.push_back(instr(RRC, 0xcb09, 8, val(C), BIT_16));
  list.push_back(instr(RRC, 0xcb0a, 8, val(D), BIT_16));
  list.push_back(instr(RRC, 0xcb0b, 8, val(E), BIT_16));
  list.push_back(instr(RRC, 0xcb0c, 8, val(H), BIT_16));
  list.push_back(instr(RRC, 0xcb0d, 8, val(L), BIT_16));
  list.push_back(instr(RRC, 0xcb0e, 16, ptr(HL), BIT_16));

  list.push_back(instr(RR, 0xcb1f, 8, val(A), BIT_16));
  list.push_back(instr(RR, 0xcb18, 8, val(B), BIT_16));
  list.push_back(instr(RR, 0xcb19, 8, val(C), BIT_16));
  list.push_back(instr(RR, 0xcb1a, 8, val(D), BIT_16));
  list.push_back(instr(RR, 0xcb1b, 8, val(E), BIT_16));
  list.push_back(instr(RR, 0xcb1c, 8, val(H), BIT_16));
  list.push_back(instr(RR, 0xcb1d, 8, val(L), BIT_16));
  list.push_back(instr(RR, 0xcb1e, 16, ptr(HL), BIT_16));

  list.push_back(instr(SLA, 0xcb27, 8, val(A), BIT_16));
  list.push_back(instr(SLA, 0xcb20, 8, val(B), BIT_16));
  list.push_back(instr(SLA, 0xcb21, 8, val(C), BIT_16));
  list.push_back(instr(SLA, 0xcb22, 8, val(D), BIT_16));
  list.push_back(instr(SLA, 0xcb23, 8, val(E), BIT_16));
  list.push_back(instr(SLA, 0xcb24, 8, val(H), BIT_16));
  list.push_back(instr(SLA, 0xcb25, 8, val(L), BIT_16));
  list.push_back(instr(SLA, 0xcb26, 16, ptr(HL), BIT_16));

  list.push_back(instr(SRA, 0xcb2f, 8, val(A), BIT_16));
  list.push_back(instr(SRA, 0xcb28, 8, val(B), BIT_16));
  list.push_back(instr(SRA, 0xcb29, 8, val(C), BIT_16));
  list.push_back(instr(SRA, 0xcb2a, 8, val(D), BIT_16));
  list.push_back(instr(SRA, 0xcb2b, 8, val(E), BIT_16));
  list.push_back(instr(SRA, 0xcb2c, 8, val(H), BIT_16));
  list.push_back(instr(SRA, 0xcb2d, 8, val(L), BIT_16));
  list.push_back(instr(SRA, 0xcb2e, 16, ptr(HL), BIT_16));

  list.push_back(instr(SRL, 0xcb3f, 8, val(A), BIT_16));
  list.push_back(instr(SRL, 0xcb38, 8, val(B), BIT_16));
  list.push_back(instr(SRL, 0xcb39, 8, val(C), BIT_16));
  list.push_back(instr(SRL, 0xcb3a, 8, val(D), BIT_16));
  list.push_back(instr(SRL, 0xcb3b, 8, val(E), BIT_16));
  list.push_back(instr(SRL, 0xcb3c, 8, val(H), BIT_16));
  list.push_back(instr(SRL, 0xcb3d, 8, val(L), BIT_16));
  list.push_back(instr(SRL, 0xcb3e, 16, ptr(HL), BIT_16));

  list.push_back(instr(BIT, 0xcb47, 8, bit(), val(A), BIT_16));
  list.push_back(instr(BIT, 0xcb40, 8, bit(), val(B), BIT_16));
  list.push_back(instr(BIT, 0xcb41, 8, bit(), val(C), BIT_16));
  list.push_back(instr(BIT, 0xcb42, 8, bit(), val(D), BIT_16));
  list.push_back(instr(BIT, 0xcb43, 8, bit(), val(E), BIT_16));
  list.push_back(instr(BIT, 0xcb44, 8, bit(), val(H), BIT_16));
  list.push_back(instr(BIT, 0xcb45, 8, bit(), val(L), BIT_16));
  list.push_back(instr(BIT, 0xcb46, 16, bit(), ptr(HL), BIT_16));

  list.push_back(instr(SET, 0xcbc7, 8, bit(), val(A), BIT_16));
  list.push_back(instr(SET, 0xcbc0, 8, bit(), val(B), BIT_16));
  list.push_back(instr(SET, 0xcbc1, 8, bit(), val(C), BIT_16));
  list.push_back(instr(SET, 0xcbc2, 8, bit(), val(D), BIT_16));
  list.push_back(instr(SET, 0xcbc3, 8, bit(), val(E), BIT_16));
  list.push_back(instr(SET, 0xcbc4, 8, bit(), val(H), BIT_16));
  list.push_back(instr(SET, 0xcbc5, 8, bit(), val(L), BIT_16));
  list.push_back(instr(SET, 0xcbc6, 16, bit(), ptr(HL), BIT_16));

  list.push_back(instr(RES, 0xcb87, 8, bit(), val(A), BIT_16));
  list.push_back(instr(RES, 0xcb80, 8, bit(), val(B), BIT_16));
  list.push_back(instr(RES, 0xcb81, 8, bit(), val(C), BIT_16));
  list.push_back(instr(RES, 0xcb82, 8, bit(), val(D), BIT_16));
  list.push_back(instr(RES, 0xcb83, 8, bit(), val(E), BIT_16));
  list.push_back(instr(RES, 0xcb84, 8, bit(), val(H), BIT_16));
  list.push_back(instr(RES, 0xcb85, 8, bit(), val(L), BIT_16));
  list.push_back(instr(RES, 0xcb86, 16, bit(), ptr(HL), BIT_16));

  list.push_back(jump(JP, 0xc3, 12, val(BIT_16)));

  list.push_back(jump(JP, 0xc2, 12, val(ZN), val(BIT_16)));
  list.push_back(jump(JP, 0xca, 12, val(ZF), val(BIT_16)));
  list.push_back(jump(JP, 0xd2, 12, val(CN), val(BIT_16)));
  list.push_back(jump(JP, 0xda, 12, val(CF), val(BIT_16)));

  list.push_back(jump(JP, 0xe9, 4, ptr(HL)));

  list.push_back(jump(JR, 0x18, 8, val(BIT_8)));

  list.push_back(jump(JR, 0x20, 8, val(ZN), val(BIT_8)));
  list.push_back(jump(JR, 0x28, 8, val(ZF), val(BIT_8)));
  list.push_back(jump(JR, 0x30, 8, val(CN), val(BIT_8)));
  list.push_back(jump(JR, 0x38, 8, val(CF), val(BIT_8)));

  list.push_back(jump(CALL, 0xcd, 12, val(BIT_16)));

  list.push_back(jump(CALL, 0xc4, 12, val(ZN), val(BIT_16)));
  list.push_back(jump(CALL, 0xcc, 12, val(ZF), val(BIT_16)));
  list.push_back(jump(CALL, 0xd4, 12, val(CN), val(BIT_16)));
  list.push_back(jump(CALL, 0xdc, 12, val(CF), val(BIT_16)));

  list.push_back(jump(RST, 0xc7, 32, con(0x00)));
  list.push_back(jump(RST, 0xcf, 32, con(0x08)));
  list.push_back(jump(RST, 0xd7, 32, con(0x10)));
  list.push_back(jump(RST, 0xdf, 32, con(0x18)));
  list.push_back(jump(RST, 0xe7, 32, con(0x20)));
  list.push_back(jump(RST, 0xef, 32, con(0x28)));
  list.push_back(jump(RST, 0xf7, 32, con(0x30)));
  list.push_back(jump(RST, 0xff, 32, con(0x38)));

  list.push_back(jump(RET, 0xc9, 8));

  list.push_back(jump(RET, 0xc0, 8, val(ZN)));
  list.push_back(jump(RET, 0xc8, 8, val(ZF)));
  list.push_back(jump(RET, 0xd0, 8, val(CN)));
  list.push_back(jump(RET, 0xd8, 8, val(CF)));

  list.push_back(jump(RETI, 0xd9, 8));
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
} // namespace backend
