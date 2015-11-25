/* Enable C++ scanner class generation. */
%option c++

/* Name of the scanner class to be AsmFlexLexer. */
%option prefix="Asm"

/* Name of the class to have yylex() defined for. */
%option yyclass="backend::compiler::AsmLexer"

%{
#include <cstdlib>

#include "cc/backend/compiler/asm_lexer.h"
#include "cc/backend/compiler/tokens.h"
#include "cc/backend/decompiler/instruction.h"

using namespace backend::decompiler;
using namespace backend::compiler;

using std::string;
%}

adc  ADC
add  ADD
and  AND
bit  BIT
call CALL
ccf  CCF
cp   CP
cpl  CPL
daa  DAA
dec  DEC
di   DI
ei   EI
halt HALT
inc  INC
jp   JP
jr   JR
ld   LD
ldd  LDD
ldhl LDHL
ldi  LDI
nop  NOP
or   OR
pop  POP
push PUSH
res  RES
ret  RET
reti RETI
rl   RL
rla  RLA
rlc  RLC
rlca RLCA
rr   RR
rra  RRA
rrc  RRC
rrca RRCA
rst  RST
sbc  SBC
scf  SCF
set  SET
sla  SLA
sra  SRA
srl  SRL
stop STOP
sub  SUB
swap SWAP
xor  XOR

a    A 
b    B 
c    C 
d    D 
e    E 
h    H 
l    L 
af   AF
bc   BC
de   DE
hl   HL
sp   SP
pc   PC
zf   ZF
nf   NF
hf   HF
cf   CF
zn   ZN
nn   NN
hn   HN
cn   CN

digit  [0-9]
number {digit}[xX]{digit}+

ignore [, \t]

%%

{adc}  { current_token_value_ = TokenValue(Opcode::ADC); return static_cast<int>(TokenType::OPCODE); }
{add}  { current_token_value_ = TokenValue(Opcode::ADD); return static_cast<int>(TokenType::OPCODE); }
{and}  { current_token_value_ = TokenValue(Opcode::AND); return static_cast<int>(TokenType::OPCODE); }
{bit}  { current_token_value_ = TokenValue(Opcode::BIT); return static_cast<int>(TokenType::OPCODE); }
{call} { current_token_value_ = TokenValue(Opcode::CALL); return static_cast<int>(TokenType::OPCODE); }
{ccf}  { current_token_value_ = TokenValue(Opcode::CCF); return static_cast<int>(TokenType::OPCODE); }
{cp}   { current_token_value_ = TokenValue(Opcode::CP); return static_cast<int>(TokenType::OPCODE); }
{cpl}  { current_token_value_ = TokenValue(Opcode::CPL); return static_cast<int>(TokenType::OPCODE); }
{daa}  { current_token_value_ = TokenValue(Opcode::DAA); return static_cast<int>(TokenType::OPCODE); }
{dec}  { current_token_value_ = TokenValue(Opcode::DEC); return static_cast<int>(TokenType::OPCODE); }
{di}   { current_token_value_ = TokenValue(Opcode::DI); return static_cast<int>(TokenType::OPCODE); }
{ei}   { current_token_value_ = TokenValue(Opcode::EI); return static_cast<int>(TokenType::OPCODE); }
{halt} { current_token_value_ = TokenValue(Opcode::HALT); return static_cast<int>(TokenType::OPCODE); }
{inc}  { current_token_value_ = TokenValue(Opcode::INC); return static_cast<int>(TokenType::OPCODE); }
{jp}   { current_token_value_ = TokenValue(Opcode::JP); return static_cast<int>(TokenType::OPCODE); }
{jr}   { current_token_value_ = TokenValue(Opcode::JR); return static_cast<int>(TokenType::OPCODE); }
{ld}   { current_token_value_ = TokenValue(Opcode::LD); return static_cast<int>(TokenType::OPCODE); }
{ldd}  { current_token_value_ = TokenValue(Opcode::LDD); return static_cast<int>(TokenType::OPCODE); }
{ldhl} { current_token_value_ = TokenValue(Opcode::LDHL); return static_cast<int>(TokenType::OPCODE); }
{ldi}  { current_token_value_ = TokenValue(Opcode::LDI); return static_cast<int>(TokenType::OPCODE); }
{nop}  { current_token_value_ = TokenValue(Opcode::NOP); return static_cast<int>(TokenType::OPCODE); }
{or}   { current_token_value_ = TokenValue(Opcode::OR); return static_cast<int>(TokenType::OPCODE); }
{pop}  { current_token_value_ = TokenValue(Opcode::POP); return static_cast<int>(TokenType::OPCODE); }
{push} { current_token_value_ = TokenValue(Opcode::PUSH); return static_cast<int>(TokenType::OPCODE); }
{res}  { current_token_value_ = TokenValue(Opcode::RES); return static_cast<int>(TokenType::OPCODE); }
{ret}  { current_token_value_ = TokenValue(Opcode::RET); return static_cast<int>(TokenType::OPCODE); }
{reti} { current_token_value_ = TokenValue(Opcode::RETI); return static_cast<int>(TokenType::OPCODE); }
{rl}   { current_token_value_ = TokenValue(Opcode::RL); return static_cast<int>(TokenType::OPCODE); }
{rla}  { current_token_value_ = TokenValue(Opcode::RLA); return static_cast<int>(TokenType::OPCODE); }
{rlc}  { current_token_value_ = TokenValue(Opcode::RLC); return static_cast<int>(TokenType::OPCODE); }
{rlca} { current_token_value_ = TokenValue(Opcode::RLCA); return static_cast<int>(TokenType::OPCODE); }
{rr}   { current_token_value_ = TokenValue(Opcode::RR); return static_cast<int>(TokenType::OPCODE); }
{rra}  { current_token_value_ = TokenValue(Opcode::RRA); return static_cast<int>(TokenType::OPCODE); }
{rrc}  { current_token_value_ = TokenValue(Opcode::RRC); return static_cast<int>(TokenType::OPCODE); }
{rrca} { current_token_value_ = TokenValue(Opcode::RRCA); return static_cast<int>(TokenType::OPCODE); }
{rst}  { current_token_value_ = TokenValue(Opcode::RST); return static_cast<int>(TokenType::OPCODE); }
{sbc}  { current_token_value_ = TokenValue(Opcode::SBC); return static_cast<int>(TokenType::OPCODE); }
{scf}  { current_token_value_ = TokenValue(Opcode::SCF); return static_cast<int>(TokenType::OPCODE); }
{set}  { current_token_value_ = TokenValue(Opcode::SET); return static_cast<int>(TokenType::OPCODE); }
{sla}  { current_token_value_ = TokenValue(Opcode::SLA); return static_cast<int>(TokenType::OPCODE); }
{sra}  { current_token_value_ = TokenValue(Opcode::SRA); return static_cast<int>(TokenType::OPCODE); }
{srl}  { current_token_value_ = TokenValue(Opcode::SRL); return static_cast<int>(TokenType::OPCODE); }
{stop} { current_token_value_ = TokenValue(Opcode::STOP); return static_cast<int>(TokenType::OPCODE); }
{sub}  { current_token_value_ = TokenValue(Opcode::SUB); return static_cast<int>(TokenType::OPCODE); }
{swap} { current_token_value_ = TokenValue(Opcode::SWAP); return static_cast<int>(TokenType::OPCODE); }
{xor}  { current_token_value_ = TokenValue(Opcode::XOR); return static_cast<int>(TokenType::OPCODE); }

{a}    { current_token_value_ = TokenValue(Register::A); return static_cast<int>(TokenType::REGISTER); }
{b}    { current_token_value_ = TokenValue(Register::B); return static_cast<int>(TokenType::REGISTER); }
{c}    { current_token_value_ = TokenValue(Register::C); return static_cast<int>(TokenType::REGISTER); }
{d}    { current_token_value_ = TokenValue(Register::D); return static_cast<int>(TokenType::REGISTER); }
{e}    { current_token_value_ = TokenValue(Register::E); return static_cast<int>(TokenType::REGISTER); }
{h}    { current_token_value_ = TokenValue(Register::H); return static_cast<int>(TokenType::REGISTER); }
{l}    { current_token_value_ = TokenValue(Register::L); return static_cast<int>(TokenType::REGISTER); }
{af}   { current_token_value_ = TokenValue(Register::AF); return static_cast<int>(TokenType::REGISTER); }
{bc}   { current_token_value_ = TokenValue(Register::BC); return static_cast<int>(TokenType::REGISTER); }
{de}   { current_token_value_ = TokenValue(Register::DE); return static_cast<int>(TokenType::REGISTER); }
{hl}   { current_token_value_ = TokenValue(Register::HL); return static_cast<int>(TokenType::REGISTER); }
{sp}   { current_token_value_ = TokenValue(Register::SP); return static_cast<int>(TokenType::REGISTER); }
{pc}   { current_token_value_ = TokenValue(Register::PC); return static_cast<int>(TokenType::REGISTER); }
{zf}   { current_token_value_ = TokenValue(Register::ZF); return static_cast<int>(TokenType::REGISTER); }
{nf}   { current_token_value_ = TokenValue(Register::NF); return static_cast<int>(TokenType::REGISTER); }
{hf}   { current_token_value_ = TokenValue(Register::HF); return static_cast<int>(TokenType::REGISTER); }
{cf}   { current_token_value_ = TokenValue(Register::CF); return static_cast<int>(TokenType::REGISTER); }
{zn}   { current_token_value_ = TokenValue(Register::ZN); return static_cast<int>(TokenType::REGISTER); }
{nn}   { current_token_value_ = TokenValue(Register::NN); return static_cast<int>(TokenType::REGISTER); }
{hn}   { current_token_value_ = TokenValue(Register::HN); return static_cast<int>(TokenType::REGISTER); }
{cn}   { current_token_value_ = TokenValue(Register::CN); return static_cast<int>(TokenType::REGISTER); }

{number} {
  current_token_value_ = TokenValue(static_cast<uint16_t>(
      atoi(string(YYText(), YYLeng()).c_str())
      ));
  return static_cast<int>(TokenType::NUMBER);
}

{ignore} // Do nothing.

%%
