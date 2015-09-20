#ifndef TURBO_SANTA_COMMON_BACK_END_DEBUG_MEMORY_PROFILER_MEMORY_PROFILER_H_
#define TURBO_SANTA_COMMON_BACK_END_DEBUG_MEMORY_PROFILER_MEMORY_PROFILER_H_

#include "cc/backend/debug/consumer.h"
#include "cc/backend/debug/filter.h"
#include "cc/backend/debug/memory_profiler/memory_access_filter.h"

namespace backend {
namespace debug {
namespace memory_profiler {

class MemoryProfiler : public Consumer {
 public:
  void Run() override;

  FilterBase* filter_external() override { return &filter_; }
 private:
  MemoryAccessFilter filter_;
};

} // namespace memory_profiler
} // namespace debug
} // namespace backend

#endif // TURBO_SANTA_COMMON_BACK_END_DEBUG_MEMORY_PROFILER_MEMORY_PROFILER_H_
