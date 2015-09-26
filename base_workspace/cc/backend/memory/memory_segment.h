#ifndef TURBO_SANTA_COMMON_BACK_END_MEMORY_MEMORY_SEGMENT_H_
#define TURBO_SANTA_COMMON_BACK_END_MEMORY_MEMORY_SEGMENT_H_

#include <functional>
#include <vector>

namespace backend {
namespace memory {

class MemorySegment {
 public:
  // Whether this is the memory segment that the address is in.
  virtual bool InRange(unsigned short address) = 0;

  // Read the value at this memory address.
  virtual unsigned char Read(unsigned short address) = 0;

  // Write this value to this memory address.
  virtual void Write(unsigned short address, unsigned char value) = 0;

  virtual void ForceWrite(unsigned short address, unsigned char value) {
    Write(address, value);
  }
};

// TODO: add a default Read() and Write()
class ContiguousMemorySegment : public MemorySegment {
 public:
  virtual bool InRange(unsigned short address) {
    return lower_address_bound() <= address && address <= upper_address_bound();
  }

 protected:
  // Inclusive lower bound of this memory segment.
  virtual unsigned short lower_address_bound() = 0;

  // Inclusive upper bound of this memory segment.
  virtual unsigned short upper_address_bound() = 0;
};

class SingleAddressSegment : public MemorySegment {
 public:
  SingleAddressSegment(unsigned short address) : address_(address) {}

  virtual bool InRange(unsigned short address) { return address == address_; }

  unsigned short address() { return address_; }

 private:
  unsigned short address_;
};

// For memory segments that we have not written yet.
class NullMemorySegment : public ContiguousMemorySegment {
 public:
  NullMemorySegment(unsigned short lower_address_bound,
                    unsigned short upper_address_bound) 
      : lower_address_bound_(lower_address_bound),
      upper_address_bound_(upper_address_bound) {}

  virtual unsigned char Read(unsigned short) { return 0; }

  virtual void Write(unsigned short, unsigned char) {}

 protected:
  virtual unsigned short lower_address_bound() { return lower_address_bound_; }
  virtual unsigned short upper_address_bound() { return upper_address_bound_; }

 private:
  unsigned short lower_address_bound_;
  unsigned short upper_address_bound_;
};

} // namespace memory
} // namespace backend
#endif // TURBO_SANTA_COMMON_BACK_END_MEMORY_MEMORY_SEGMENT_H_
