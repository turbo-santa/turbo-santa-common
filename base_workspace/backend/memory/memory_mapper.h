#ifndef TURBO_SANTA_COMMON_BACK_END_MEMORY_MEMORY_MAPPER_H_
#define TURBO_SANTA_COMMON_BACK_END_MEMORY_MEMORY_MAPPER_H_

#include "backend/config.h"

#include <vector>
#include "backend/memory/flags.h"
#include "backend/memory/flag_container.h"
#include "backend/memory/memory_segment.h"
#include "backend/memory/module.h"

namespace back_end {
namespace memory {

class MemoryMapper {
 public:
  unsigned char Read(unsigned short address);
  void Write(unsigned short address, unsigned char value);
  void RegisterModule(const Module& module);

 private:
  FlagContainer flag_container_;
  std::vector<MemorySegment*> memory_segments_ = std::vector<MemorySegment*>(1, &flag_container_);
};

} // namespace memory
} // namespace back_end
#endif // TURBO_SANTA_COMMON_BACK_END_MEMORY_MEMORY_MAPPER_H_
