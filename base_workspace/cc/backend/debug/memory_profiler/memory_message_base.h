#ifndef TURBO_SANTA_COMMON_BACK_END_DEBUG_MEMORY_PROFILER_MEMORY_MESSAGE_BASE_H_
#define TURBO_SANTA_COMMON_BACK_END_DEBUG_MEMORY_PROFILER_MEMORY_MESSAGE_BASE_H_

#include "cc/backend/debug/message.h"

namespace backend {
namespace debug {
namespace memory_profiler {

class MemoryMessageBase : public backend::debug::Message {
 public:
  enum Type {
    MEMORY_ACCESS,
  };

  virtual Type type() const = 0;

  BaseType base_type() const override { return MEMORY; }
};

} // namespace memory_profiler
} // namespace debug
} // namespace backend

#endif // TURBO_SANTA_COMMON_BACK_END_DEBUG_MEMORY_PROFILER_MEMORY_MESSAGE_BASE_H_
