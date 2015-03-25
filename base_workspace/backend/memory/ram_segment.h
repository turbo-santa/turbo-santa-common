#ifndef TURBO_SANTA_COMMON_BACK_END_MEMORY_RAM_SEGMENT_H_
#define TURBO_SANTA_COMMON_BACK_END_MEMORY_RAM_SEGMENT_H_

#include "back_end/config.h"

#include "back_end/memory/memory_segment.h"

namespace back_end {
namespace memory {

class RAMSegment : public ContiguousMemorySegment {
 public:
  RAMSegment(unsigned short lower_address_bound, unsigned short upper_address_bound) :
      lower_address_bound_(lower_address_bound), 
      upper_address_bound_(upper_address_bound), 
      memory_(upper_address_bound - lower_address_bound + 1, 0x00) {}

  virtual unsigned char Read(unsigned short address) {
    return memory_[address - lower_address_bound_];
  }

  virtual void Write(unsigned short address, unsigned char value) {
    memory_[address - lower_address_bound_] = value;
  }

 protected:
  unsigned short lower_address_bound_;
  unsigned short upper_address_bound_;
  std::vector<unsigned char> memory_;

  virtual unsigned short lower_address_bound() { return lower_address_bound_; }
  virtual unsigned short upper_address_bound() { return upper_address_bound_; }
};

} // namespace memory
} // namespace back_end

#endif // TURBO_SANTA_COMMON_BACK_END_MEMORY_RAM_SEGMENT_H_
