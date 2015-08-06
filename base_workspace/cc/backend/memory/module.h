#ifndef TURBO_SANTA_COMMON_BACK_END_MEMORY_MODULE_H_
#define TURBO_SANTA_COMMON_BACK_END_MEMORY_MODULE_H_

#include <vector>
#include "cc/backend/memory/memory_segment.h"
#include "cc/backend/memory/flags.h"

namespace back_end {
namespace memory {

class Module {
 public:
  const std::vector<MemorySegment*>& memory_segments() const { return memory_segments_; }
  const std::vector<Flag*>& flags() const { return flags_; }

 protected:
  void add_memory_segment(MemorySegment* memory_segment) { memory_segments_.push_back(memory_segment); }
  void add_flag(Flag* flag) { flags_.push_back(flag); }

 private:
  std::vector<MemorySegment*> memory_segments_;
  std::vector<Flag*> flags_;
};

} // namespace memory
} // namespace back_end

#endif // TURBO_SANTA_COMMON_BACK_END_MEMORY_MODULE_H_
