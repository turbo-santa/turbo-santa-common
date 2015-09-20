#include "cc/backend/debug/publisher.h"

#include <memory>
#include <vector>

namespace backend {
namespace debug {

using std::unique_ptr;
using std::vector;

void Publisher::Publish(unique_ptr<Message> message) {
  if (forwarder_ != nullptr) {
    forwarder_->Publish(std::move(message));
  }
}

} // namespace debug
} // namespace backend
