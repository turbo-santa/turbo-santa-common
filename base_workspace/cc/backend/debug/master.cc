#include "cc/backend/debug/master.h"

namespace backend {
namespace debug {

using std::shared_ptr;
using std::unique_ptr;

void Master::SendToConsumers(unique_ptr<Message> message) {
  shared_ptr<Message> shared_message(message.release());

  for (ConsumerRunner& consumer : consumers_) {
    consumer.filter()->PutMessage(shared_message);
  }
}

void Master::Register(unique_ptr<Consumer> consumer) {
  // Consumers are not copyable.
  consumers_.emplace_back(std::move(consumer));
  consumers_.end()->Exec();
}

} // namespace debug
} // namespace backend
