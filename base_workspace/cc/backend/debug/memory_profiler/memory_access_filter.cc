#include "cc/backend/debug/memory_profiler/memory_access_filter.h"

namespace backend {
namespace debug {
namespace memory_profiler {

using utility::None;
using utility::Option;
using utility::Some;

Option<const MemoryAccess*> MemoryAccessFilter::Apply(const Message* message) {
  if (message->base_type() == Message::MEMORY) {
    const MemoryMessageBase* memory_message = 
        static_cast<const MemoryMessageBase*>(message);
    if (memory_message->type() == MemoryMessageBase::MEMORY_ACCESS) {
      return Some(static_cast<const MemoryAccess*>(memory_message));
    }
  }
  return None<const MemoryAccess*>();
}

} // namespace memory_profiler
} // namespace debug
} // namespace backend
