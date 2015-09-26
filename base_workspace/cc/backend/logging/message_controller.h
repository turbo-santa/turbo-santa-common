#ifndef TURBO_SANTA_COMMON_BACK_END_LOGGING_MESSAGE_CONTROLLER_H_
#define TURBO_SANTA_COMMON_BACK_END_LOGGING_MESSAGE_CONTROLLER_H_

#include <memory>
#include <vector>
#include "cc/backend/logging/communication_layer.h"
#include "protobuf/backend/logging/message.pb.h"

namespace backend {
namespace logging {

class MessageController {
 public:
  void SendToConsumers(std::unique_ptr<Message> message);

  CommunicationLayer* communication_layer() { 
    return communication_layer_.get();
  }

  void Publish(std::unique_ptr<Message> message) {
    SendToConsumers(std::move(message)); 
  }

  static MessageController* GetInstance() {
    static MessageController controller;
    return &controller;
  }

 private:
  std::unique_ptr<CommunicationLayer> communication_layer_;

  MessageController() : communication_layer_(new CommunicationLayer()) {}
  MessageController(const MessageController&) = delete;
  void operator=(const MessageController&) = delete;
};

} // namespace logging
} // namespace backend

#ifdef LOGGING_ENABLED
  #define PUBLISH(message) \
      backend::logging::MessageController::GetInstance()->Publish(message);
#else
  #define PUBLISH(message)
#endif

#endif // TURBO_SANTA_COMMON_BACK_END_LOGGING_MESSAGE_CONTROLLER_H_
