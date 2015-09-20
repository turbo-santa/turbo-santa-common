#ifndef TURBO_SANTA_COMMON_BACK_END_DEBUG_PUBLISHER_H_
#define TURBO_SANTA_COMMON_BACK_END_DEBUG_PUBLISHER_H_

#include <memory>
#include <vector>
#include "cc/backend/debug/message.h"

namespace backend {
namespace debug {

class Publisher {
 public:
  virtual void Publish(std::unique_ptr<Message> message);
  // Forward all messages to publisher.
  virtual void ForwardTo(Publisher* publisher) { forwarder_ = publisher; }
  // Have publisher forward to this.
  virtual void Own(Publisher* publisher) { publisher->ForwardTo(this); }

 private:
  Publisher* forwarder_ = nullptr;
  std::vector<std::unique_ptr<Message>> messages_;
};

} // namespace debug 
} // namespace backend

// If DEBUG is not defined, we do not want to publish messages since the
// messages.
#ifdef DEBUG
  #define PUBLISH(message) Publish(message);
#else
  #define PUBLISH(message)
#endif

#endif // TURBO_SANTA_COMMON_BACK_END_DEBUG_PUBLISHER_H_
