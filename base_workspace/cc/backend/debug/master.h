#ifndef TURBO_SANTA_COMMON_BACK_END_DEBUG_MASTER_H_
#define TURBO_SANTA_COMMON_BACK_END_DEBUG_MASTER_H_

#include <memory>
#include <vector>
#include "cc/backend/debug/consumer.h"
#include "cc/backend/debug/consumer_runner.h"
#include "cc/backend/debug/message.h"
#include "cc/backend/debug/publisher.h"

namespace backend {
namespace debug {

class Master : public Publisher {
 public:
  void SendToConsumers(std::unique_ptr<Message> message);

  // NOT THREAD SAFE!
  void Register(std::unique_ptr<Consumer> consumer);

  void Publish(std::unique_ptr<Message> message) override {
    SendToConsumers(std::move(message)); 
  }

  void ForwardTo(Publisher*) override {};
 private:
  std::vector<ConsumerRunner> consumers_;
};

} // namespace debug
} // namespace backend

#endif // TURBO_SANTA_COMMON_BACK_END_DEBUG_MASTER_H_
