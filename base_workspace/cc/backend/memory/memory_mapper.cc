#include "cc/backend/memory/memory_mapper.h"
#include "cc/backend/logging/logging.h"
#include "glog/logging.h"

namespace backend {
namespace memory {

using std::vector;

namespace {
MemorySegment* Lookup(const vector<MemorySegment*>& segments, unsigned short address) {
  for (MemorySegment* segment : segments) {
    if (segment->InRange(address)) {
      return segment;
    }
  }
  LOG(FATAL) << "Address out of range: 0x" << std::hex << address;
}
} // namespace

void MemoryMapper::RegisterModule(const Module& module) {
  for (MemorySegment* segment : module.memory_segments()) {
    memory_segments_.push_back(segment);
  }

  for (Flag* flag : module.flags()) {
    if (flag->address() == 0xffff) {
      // The interrupt enable flag, address 0xffff, does not lie within the
      // usual flag address space.
      memory_segments_.push_back(flag);
      LOG(INFO) << "Adding interrupt enable.";
    } else {
      flag_container_.add_flag(flag);
    }
  }
}

unsigned char MemoryMapper::Read(unsigned short address) {
  PUBLISH_READ(address, Lookup(memory_segments_, address)->Read(address));
  return Lookup(memory_segments_, address)->Read(address);
}

void MemoryMapper::Write(unsigned short address, unsigned char value) {
  PUBLISH_WRITE(address, Lookup(memory_segments_, address)->Read(address), value);
  Lookup(memory_segments_, address)->Write(address, value);
}

void MemoryMapper::ForceWrite(unsigned short address, unsigned char value) {
  Lookup(memory_segments_, address)->ForceWrite(address, value);
}

} // namespace memory
} // namespace backend
