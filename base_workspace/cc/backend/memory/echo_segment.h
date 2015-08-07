#ifndef TURBO_SANTA_COMMON_BACK_END_MEMORY_ECHO_SEGMENT_H_
#define TURBO_SANTA_COMMON_BACK_END_MEMORY_ECHO_SEGMENT_H_

#include "cc/backend/memory/memory_segment.h"
#include "cc/backend/memory/ram_segment.h"
#include "submodules/glog/src/glog/logging.h"

namespace backend {
namespace memory {

class EchoSegment : public ContiguousMemorySegment {
 public:
  EchoSegment(RAMSegment* internal_ram_0, RAMSegment* internal_ram_1) :
      internal_ram_0_(internal_ram_0), internal_ram_1_(internal_ram_1) {}

  virtual unsigned char Read(unsigned short address) {
    if (internal_ram_0_->InRange(translate_address(address))) {
      return internal_ram_0_->Read(translate_address(address));
    } else if (internal_ram_1_->InRange(translate_address(address))) {
      return internal_ram_1_->Read(translate_address(address));
    } else {
      LOG(FATAL) << "Unexpected address: " << std::hex << address << " translated address: " << std::hex << translate_address(address);
    }
  }

  virtual void Write(unsigned short address, unsigned char value) {
    if (internal_ram_0_->InRange(translate_address(address))) {
      internal_ram_0_->Write(translate_address(address), value);
    } else if (internal_ram_1_->InRange(translate_address(address))) {
      internal_ram_1_->Write(translate_address(address), value);
    } else {
      LOG(FATAL) << "Unexpected address: " << std::hex << address;
    }
  }

 protected:
  virtual unsigned short lower_address_bound() { return 0xe000; }
  virtual unsigned short upper_address_bound() { return 0xfdff; }

 private:
  RAMSegment* internal_ram_0_;
  RAMSegment* internal_ram_1_;

  virtual unsigned short translate_address(unsigned short address) {
    return address - (lower_address_bound() - 0xc000);
  }
};

} // namespace memory
} // namespace backend

#endif // TURBO_SANTA_COMMON_BACK_END_MEMORY_ECHO_SEGMENT_H_
