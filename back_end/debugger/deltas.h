#ifndef TURBO_SANTA_COMMON_BACK_END_DEBUGGER_DELTAS_H_
#define TURBO_SANTA_COMMON_BACK_END_DEBUGGER_DELTAS_H_

#include "back_end/config.h"

#include <vector>

namespace back_end {
namespace debugger {

struct RegisterDelta {
  enum RegisterName {
    A,
    F,
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
  virtual std::vector<RegisterDelta>&& RetrieveDelta() = 0;
};

class MemoryProducer {
  virtual std::vector<MemoryDelta>&& RetrieveDelta() = 0;
};

class PCProducer {
  virtual PCDelta RetrieveDelta() = 0;
};

} // namespace debugger
} // namespace back_end

#endif // TURBO_SANTA_COMMON_BACK_END_DEBUGGER_DELTAS_H_
