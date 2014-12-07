#include "back_end/debugger/deltas.h"

namespace back_end {
namespace debugger {
using std::vector;

std::string RegisterDelta::GetName() {
  switch(name) {
    case(A):
      return "A";
    case(FZ):
      return "FZ";
    case(FN):
      return "FN";
    case(FH):
      return "FH";
    case(FC):
      return "FC";
    case(B):
      return "B";
    case(C):
      return "C";
    case(D):
      return "D";
    case(E):
      return "E";
    case(H):
      return "H";
    case(L):
      return "L";
    case(PC):
      return "PC";
    case(SP):
      return "SP";
    default:
      return "Dafuck";
  }
}

vector<RegisterDelta> RegisterProducer::RetrieveDelta() {
  vector<RegisterDelta> deltas;

  if (previous_cpu_.flag_struct.rA != current_cpu_->flag_struct.rA) {
    deltas.push_back({RegisterDelta::A, previous_cpu_.flag_struct.rA, current_cpu_->flag_struct.rA});
    previous_cpu_.flag_struct.rA = current_cpu_->flag_struct.rA;
  }
  
  if (previous_cpu_.flag_struct.rF.Z != current_cpu_->flag_struct.rF.Z) {
    deltas.push_back({RegisterDelta::FZ, static_cast<unsigned short>(previous_cpu_.flag_struct.rF.Z), static_cast<unsigned short>(current_cpu_->flag_struct.rF.Z)});
    previous_cpu_.flag_struct.rF.Z = current_cpu_->flag_struct.rF.Z;
  }
  
  if (previous_cpu_.flag_struct.rF.N != current_cpu_->flag_struct.rF.N) {
    deltas.push_back({RegisterDelta::FN, static_cast<unsigned short>(previous_cpu_.flag_struct.rF.N), static_cast<unsigned short>(current_cpu_->flag_struct.rF.N)});
    previous_cpu_.flag_struct.rF.N = current_cpu_->flag_struct.rF.N;
  }
  
  if (previous_cpu_.flag_struct.rF.H != current_cpu_->flag_struct.rF.H) {
    deltas.push_back({RegisterDelta::FH, static_cast<unsigned short>(previous_cpu_.flag_struct.rF.H), static_cast<unsigned short>(current_cpu_->flag_struct.rF.H)});
    previous_cpu_.flag_struct.rF.H = current_cpu_->flag_struct.rF.H;
  }
  
  if (previous_cpu_.flag_struct.rF.C != current_cpu_->flag_struct.rF.C) {
    deltas.push_back({RegisterDelta::FC, static_cast<unsigned short>(previous_cpu_.flag_struct.rF.C), static_cast<unsigned short>(current_cpu_->flag_struct.rF.C)});
    previous_cpu_.flag_struct.rF.C = current_cpu_->flag_struct.rF.C;
  }
  
  if (previous_cpu_.bc_struct.rB != current_cpu_->bc_struct.rB) {
    deltas.push_back({RegisterDelta::B, previous_cpu_.bc_struct.rB, current_cpu_->bc_struct.rB});
    previous_cpu_.bc_struct.rB = current_cpu_->bc_struct.rB;
  }
  
  if (previous_cpu_.bc_struct.rC != current_cpu_->bc_struct.rC) {
    deltas.push_back({RegisterDelta::C, previous_cpu_.bc_struct.rC, current_cpu_->bc_struct.rC});
    previous_cpu_.bc_struct.rC = current_cpu_->bc_struct.rC;
  }
  
  if (previous_cpu_.de_struct.rD != current_cpu_->de_struct.rD) {
    deltas.push_back({RegisterDelta::D, previous_cpu_.de_struct.rD, current_cpu_->de_struct.rD});
    previous_cpu_.de_struct.rD = current_cpu_->de_struct.rD;
  }
  
  if (previous_cpu_.de_struct.rE != current_cpu_->de_struct.rE) {
    deltas.push_back({RegisterDelta::E, previous_cpu_.de_struct.rE, current_cpu_->de_struct.rE});
    previous_cpu_.de_struct.rE = current_cpu_->de_struct.rE;
  }
  
  if (previous_cpu_.hl_struct.rH != current_cpu_->hl_struct.rH) {
    deltas.push_back({RegisterDelta::H, previous_cpu_.hl_struct.rH, current_cpu_->hl_struct.rH});
    previous_cpu_.hl_struct.rH = current_cpu_->hl_struct.rH;
  }
  
  if (previous_cpu_.hl_struct.rL != current_cpu_->hl_struct.rL) {
    deltas.push_back({RegisterDelta::L, previous_cpu_.hl_struct.rL, current_cpu_->hl_struct.rL});
    previous_cpu_.hl_struct.rL = current_cpu_->hl_struct.rL;
  }
  
  if (previous_cpu_.rPC != current_cpu_->rPC) {
    deltas.push_back({RegisterDelta::PC, previous_cpu_.rPC, current_cpu_->rPC});
    previous_cpu_.rPC = current_cpu_->rPC;
  }
  
  if (previous_cpu_.rSP != current_cpu_->rSP) {
    deltas.push_back({RegisterDelta::SP, previous_cpu_.rSP, current_cpu_->rSP});
    previous_cpu_.rSP = current_cpu_->rSP;
  }

  return deltas;
}

} // namespace debugger
} // namespace back_end
