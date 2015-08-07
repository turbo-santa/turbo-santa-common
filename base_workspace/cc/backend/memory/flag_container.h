#ifndef TURBO_SANTA_COMMON_BACK_END_MEMORY_FLAG_CONTAINER_H_
#define TURBO_SANTA_COMMON_BACK_END_MEMORY_FLAG_CONTAINER_H_

#include <vector>
#include "cc/backend/memory/flags.h"
#include "cc/backend/memory/memory_segment.h"

// All flags, with the exception of the interrupt enable flag lie within the IO
// ports address space.

namespace backend {
namespace memory {

class FlagContainer : public ContiguousMemorySegment {
 public:
  unsigned char Read(unsigned short address) {
    for (Flag* flag : flags_) {
      if (flag->InRange(address)) {
        return flag->Read(address);
      }
    }
    return 0;
  }

  void Write(unsigned short address, unsigned char value) {
    for (Flag* flag : flags_) {
      if (flag->InRange(address)) {
        flag->Write(address, value);
        return;
      }
    }
  }

  void add_flag(Flag* flag) { flags_.push_back(flag); }

 protected:
  std::vector<Flag*> flags_;

  // Inclusive lower bound of this memory segment.
  virtual unsigned short lower_address_bound() { return 0xff00; } 

  // Inclusive upper bound of this memory segment.
  virtual unsigned short upper_address_bound() { return 0xff7f; }
};

} // namespace memory
} // namespace backend

#endif // TURBO_SANTA_COMMON_BACK_END_MEMORY_FLAG_CONTAINER_H_
