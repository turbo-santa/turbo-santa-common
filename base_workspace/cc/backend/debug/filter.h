#ifndef TURBO_SANTA_COMMON_BACK_END_DEBUG_FILTER_H_
#define TURBO_SANTA_COMMON_BACK_END_DEBUG_FILTER_H_

#include <memory>

#include "cc/backend/debug/message.h"
#include "cc/utility/option.h"
#include "cc/utility/stream.h"

namespace backend {
namespace debug {

// Base for all filters, allows any filter to be generically referenced as a
// base type.
class FilterBase {
 public:
  virtual void PutMessage(std::shared_ptr<const Message> message) = 0;
};


// Filters extract relevent data from a stream of messages. When a Consumer is
// registered with the Master, all messages will be forwarded to its filter
// which is responsible for filtering out messages which are relevent based on
// arbitrary criteria. As a conveniece, filters may downcast a message to the
// desired type of message or even select a pointer to a field inside of a
// message, but may not return data that is not owned by the message.
template <typename T>
class Filter : public FilterBase {
 public:
  // Apply should only limit the amount of data received not create new data in
  // any way. The result of Apply will be wrapped in a shared pointer that
  // references the message pointed into this and will NOT be destroyed until
  // the object output by this filter is deallocated.
  //
  // The normal use case is downcasting message to a subtype; however, the
  // filter IS allowed to extract a pointer to a member of the message.
  // Nevertheless, any and all new objects should be constructed by the owning
  // consumer.
  virtual utility::Option<const T*> Apply(const Message* message) = 0;

  void PutMessage(std::shared_ptr<const Message> message) override final {
    in_stream_.Put(message);
  }

  utility::Option<std::shared_ptr<const T>> TakeMessage() {
    std::shared_ptr<const Message> message = in_stream_.Take();
    utility::Option<const T*> result = Apply(message);
    if (result.is_present()) {
      // Here the derived_object references some part of the original message,
      // so we don't want message destroyed until we are done with it.
      std::shared_ptr<const T> derived_object(message, result.get());
      return utility::Some(derived_object);
    } else {
      return utility::None<std::shared_ptr<const T>>();
    }
  }

  void Close() { in_stream_.Close(); }

 private:
  utility::Stream<std::shared_ptr<const Message>> in_stream_;
};

} // namespace debug
} // namespace backend

#endif // TURBO_SANTA_COMMON_BACK_END_DEBUG_FILTER_H_
