#ifndef TURBO_SANTA_COMMON_BACK_END_DEBUG_CONSUMER_RUNNER_H_
#define TURBO_SANTA_COMMON_BACK_END_DEBUG_CONSUMER_RUNNER_H_

#include <memory>
#include <thread>

#include "cc/backend/debug/consumer.h"

namespace backend {
namespace debug {

class ConsumerRunner {
 public:
  ConsumerRunner(std::unique_ptr<Consumer> consumer) 
      : consumer_(std::move(consumer)) {}

  void Exec() {
    thread_ = std::thread([this]() { this->consumer_->Run(); });
  }

  FilterBase* filter() { return consumer_->filter_external(); }

 private:
  std::unique_ptr<Consumer> consumer_;
  std::thread thread_;
};

} // namespace debug
} // namespace backend
#endif // TURBO_SANTA_COMMON_BACK_END_DEBUG_CONSUMER_RUNNER_H_
