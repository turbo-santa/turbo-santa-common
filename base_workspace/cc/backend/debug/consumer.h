#ifndef TURBO_SANTA_COMMON_BACK_END_DEBUG_CONSUMER_H_
#define TURBO_SANTA_COMMON_BACK_END_DEBUG_CONSUMER_H_

#include "cc/backend/debug/filter.h"

namespace backend {
namespace debug {

class Consumer {
 public:
  virtual FilterBase* filter_external() = 0;
  virtual void Run() = 0;
};

} // namespace debug
} // namespace backend

#endif // TURBO_SANTA_COMMON_BACK_END_DEBUG_CONSUMER_H_
