#include "cc/backend/logging/message_controller.h"

namespace backend {
namespace logging {

using std::shared_ptr;
using std::unique_ptr;

void MessageController::SendToConsumers(unique_ptr<Message> message) {
  communication_layer_->PutMessage(std::move(message));
}

} // namespace logging
} // namespace backend
