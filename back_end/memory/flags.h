#ifndef TURBO_SANTA_COMMON_BACK_END_MEMORY_FLAGS_H_
#define TURBO_SANTA_COMMON_BACK_END_MEMORY_FLAGS_H_

#include "back_end/memory/memory_segment.h"

namespace back_end {
namespace memory {

class Flag : public SingleAddressSegment {
 public:
  Flag(unsigned short address) : SingleAddressSegment(address) {}
  virtual bool InRange(unsigned short address_) { return address_ == address(); }
  virtual unsigned char Read(unsigned short) { return flag_; }
  virtual void Write(unsigned short, unsigned char value) { flag_ = value; }

  virtual unsigned char flag() { return flag_; }
  virtual void set_flag(unsigned char value) { flag_ = value; }

 protected:
  // Returns whether an individual bit is set.
  bool bit(int bit) { return ((0b00000001 << bit) & flag_) != 0; }

  // Sets an indiviual bit.
  void set_bit(int bit, bool value) {
    unsigned char mask = 0b00000001 << bit;
    if (value) {
      flag_ |= mask;
    } else {
      flag_ &= ~mask;
    }
  }

 private:
  unsigned char flag_ = 0;
};

} // namespace memory
} // namespace back_end

#endif // TURBO_SANTA_COMMON_BACK_END_MEMORY_FLAGS_H_
