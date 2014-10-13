#ifndef TURBO_SANTA_COMMON_BACK_END_MEMORY_MEMORY_MAPPER_H_
#define TURBO_SANTA_COMMON_BACK_END_MEMORY_MEMORY_MAPPER_H_

#include "back_end/memory/memory_page.h"
#include "back_end/memory/memory_segment.h"

namespace back_end {
namespace memory {

class VideoRam;
class ObjectAttributeMemory;
class Unusable;
class IORegisters;
class ZeroPage;
class InteruptEnable;

class MemoryMapper {
 public:
  MemoryMapper() : mapped_memory_(new unsigned char[kMaxSize]) {}

  ~MemoryMapper() { delete mapped_memory_; }

  unsigned char* get_pointer() { return mapped_memory_; }

  void set(unsigned short location, unsigned char value) { mapped_memory_[location] = value; }


  unsigned char get(unsigned short location) { return mapped_memory_[location]; }

  unsigned long kMaxSize = 0x10000;

 private:
  unsigned char* mapped_memory_;
};

} // namespace memory_mapper
} // namespace back_end
#endif // TURBO_SANTA_COMMON_BACK_END_MEMORY_MEMORY_MAPPER_H_
