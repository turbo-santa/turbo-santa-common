#ifndef TURBO_SANTA_COMMON_BACK_END_DEBUGGER_DELTAS_H_
#define TURBO_SANTA_COMMON_BACK_END_DEBUGGER_DELTAS_H_

#include "back_end/config.h"

#include <vector>
#include "back_end/opcode_executor/registers.h"

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
  unsigned short old_value;
  unsigned short new_value;
};

struct MemoryDelta {
  unsigned short address;
  unsigned char old_value;
  unsigned char new_value;
};

struct PCDelta {
  unsigned short old_value;
  unsigned short new_value;
};

class RegisterProducer {
 public:
  virtual std::vector<RegisterDelta>&& RetrieveDelta();
 private:
  registers::GB_CPU previous_cpu_;
  registers::GB_CPU* current_cpu_;
  std::vector<RegisterDelta> register_deltas_;
};

class MemoryProducer {
 public:
  virtual std::vector<MemoryDelta>&& RetrieveDelta() {
    std::vector<MemoryDelta> temp = std::move(memory_deltas_);
    memory_deltas_ = std::vector<MemoryDelta>();
    return std::move(temp);
  }

  virtual void MemoryWrite(unsigned short address, unsigned char old_value, unsigned char new_value) {
    memory_deltas_.push_back({address, old_value, new_value});
  }
 private:
  std::vector<MemoryDelta> memory_deltas_;
};

class PCProducer {
 public:
  virtual PCDelta RetrieveDelta() {
    PCDelta ret_val = {previous_value_, *current_value_};
    previous_value_ = *current_value_;
    return ret_val;
  }
  
 private:
  unsigned short previous_value_;
  unsigned short* current_value_;
};

} // namespace debugger
} // namespace back_end

#endif // TURBO_SANTA_COMMON_BACK_END_DEBUGGER_DELTAS_H_
