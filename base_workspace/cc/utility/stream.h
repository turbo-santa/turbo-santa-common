#ifndef TURBO_SANTA_COMMON_UTILITY_STREAM_H_
#define TURBO_SANTA_COMMON_UTILITY_STREAM_H_

#include <atomic>
#include <condition_variable>
#include <list>
#include <mutex>
#include "cc/utility/option.h"

namespace utility {

// A thread-safe data structure used for passing data from one thread to
// another in a buffered manner.
template <typename T>
class Stream {
 public:
  Stream() : is_closed_(false) {}

  void Put(T t) {
    std::unique_lock<std::mutex> lock(mutex_);
    if (!is_closed_) {
      data_.push_back(t);
      condition_.notify_all();
    }
  }

  // Receives an element of type T from sender, blocks until element is received
  // or thread is closed. Returns None if stream is closed.
  Option<T> Take() {
    std::unique_lock<std::mutex> lock(mutex_);
    if (!is_closed_) {
      while (data_.empty()) {
        condition_.wait(lock);
      }
      Option<T> value = Some<T>(data_.front());
      data_.pop_front();
      return value;
    } else {
      return None<T>();
    }
  }

  bool is_closed() { return is_closed_; }

  // Closes stream.
  void Close() { is_closed_ = true; }

 private:
  std::atomic<bool> is_closed_;
  std::mutex mutex_;
  std::condition_variable condition_;
  std::list<T> data_;
};

} // namespace utility

#endif // TURBO_SANTA_COMMON_UTILITY_STREAM_H_
