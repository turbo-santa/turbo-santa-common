#ifndef TURBO_SANTA_COMMON_BACK_END_DEBUG_MEMORY_PROFILER_MEMORY_ACCESS_FILTER_H_
#define TURBO_SANTA_COMMON_BACK_END_DEBUG_MEMORY_PROFILER_MEMORY_ACCESS_FILTER_H_

#include "cc/backend/debug/filter.h"
#include "cc/backend/debug/memory_profiler/memory_access.h"
#include "cc/utility/option.h"

namespace backend {
namespace debug {
namespace memory_profiler {

class MemoryAccessFilter : public Filter<MemoryAccess> {
 public:
  utility::Option<const MemoryAccess*> Apply(const Message* message);
};

} // namespace memory_profiler
} // namespace debug
} // namespace backend

#endif // TURBO_SANTA_COMMON_BACK_END_DEBUG_MEMORY_PROFILER_MEMORY_ACCESS_FILTER_H_
