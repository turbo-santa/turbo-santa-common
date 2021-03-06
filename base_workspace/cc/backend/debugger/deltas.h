#ifndef TURBO_SANTA_COMMON_BACK_END_DEBUGGER_DELTAS_H_
#define TURBO_SANTA_COMMON_BACK_END_DEBUGGER_DELTAS_H_

#include "backend/config.h"

#include <vector>
#include "backend/debugger/bloon_filter.h"
#include "backend/opcode_executor/registers.h"

namespace back_end {
namespace debugger {

struct RegisterDelta {
  enum RegisterName {
    A,
    FZ,
    FN,
    FH,
    FC,
    B,
    C,
    D,
    E,
    H,
    L,
    PC,
    SP
  };

  RegisterName name;
  std::string GetName();
  unsigned short old_value;
  unsigned short new_value;
};

struct MemoryDelta {
  unsigned short address;
  unsigned char old_value;
  unsigned char new_value;
};

struct PCDelta {
  bool visited_before;
  unsigned short old_value;
  unsigned short new_value;
};

class RegisterProducer {
 public:
  RegisterProducer(registers::GB_CPU* current_cpu) : current_cpu_(current_cpu) {}

  virtual std::vector<RegisterDelta> RetrieveDelta();
 private:
  registers::GB_CPU previous_cpu_;
  registers::GB_CPU* current_cpu_;
  std::vector<RegisterDelta> register_deltas_;
};

class MemoryProducer {
 public:
  virtual std::vector<MemoryDelta> RetrieveDelta() {
    std::vector<MemoryDelta> temp;
    temp.swap(memory_deltas_);
    return temp;
  }

  virtual void MemoryWrite(unsigned short address, unsigned char old_value, unsigned char new_value) {
    memory_deltas_.push_back({address, old_value, new_value});
  }
 private:
  std::vector<MemoryDelta> memory_deltas_;
};

class PCProducer {
 public:
  PCProducer(unsigned short* current_value) : current_value_(current_value) {}
  virtual PCDelta RetrieveDelta() {
    PCDelta ret_val = {filter_.AddAndCheckIfVisited(previous_value_), previous_value_, *current_value_};
    previous_value_ = *current_value_;
    return ret_val;
  }

  void Clear() { filter_.Clear(); }
  
 private:
  BloonFilter filter_;
  unsigned short previous_value_ = 0;
  unsigned short* current_value_;
};

} // namespace debugger
} // namespace back_end

#endif // TURBO_SANTA_COMMON_BACK_END_DEBUGGER_DELTAS_H_
