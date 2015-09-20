#include "cc/backend/debug/memory_profiler/memory_profiler.h"

#include <memory>

#include "cc/backend/debug/memory_profiler/memory_access.h"
#include "cc/utility/option.h"
#include "glog/logging.h"

namespace backend {
namespace debug {
namespace memory_profiler {

using std::shared_ptr;
using utility::Option;

namespace {

void HandleMessage(shared_ptr<const MemoryAccess> memory_access) {
  if (memory_access->READ) {
    LOG(INFO) << "Read performed at: " << memory_access->address() 
        << ", value = " << memory_access->old_value();
  } else {
    LOG(INFO) << "Write performed at: " << memory_access->address() 
        << ", old value = " << memory_access->old_value()
        << ", new value = " << memory_access->new_value();
  }
}

} // namespace

void MemoryProfiler::Run() {
  LOG(INFO) << "Running..";
  while (!filter_.is_closed()) {
    Option<shared_ptr<const MemoryAccess>> access = filter_.TakeMessage();
    if (access.is_present()) {
      HandleMessage(access.get());
    }
  }
}

} // namespace memory_profiler
} // namespace debug
} // namespace backend

