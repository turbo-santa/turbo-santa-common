#include "cc/backend/debug/consumer_runner.h"

namespace backend {
namespace debug {

void ConsumerRunner::Exec() {
  thread_ = std::thread([this]() { this->consumer_->Run(); });
}

} // namespace debug
} // namespace backend
