#ifndef TURBO_SANTA_COMMON_UTILITY_OPTION_H_
#define TURBO_SANTA_COMMON_UTILITY_OPTION_H_

#include "glog/logging.h"

namespace utility {

template<typename T>
class Option {
 public:
  Option(T value) : is_present_(true), value_(value) {}
  Option() {}

  bool is_present() { return is_present_; }
  T get() {
    if (is_present_) {
      return value_;
    } else {
      LOG(FATAL) << "Value is not present.";
    }
  }

 private:
  bool is_present_ = false;
  T value_;
};

template<typename T>
Option<T> Some(T t) { return Option<T>(t); }

template<typename T>
Option<T> None() { return Option<T>(); }

} // namespace utility

#endif // TURBO_SANTA_COMMON_UTILITY_OPTION_H_
