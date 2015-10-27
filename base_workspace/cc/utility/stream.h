#ifndef TURBO_SANTA_COMMON_UTILITY_STREAM_H_
#define TURBO_SANTA_COMMON_UTILITY_STREAM_H_

#include <atomic>
#include <condition_variable>
#include <memory>
#include <mutex>
#include "cc/utility/resizable_ring_buffer.h"

namespace utility {

// A thread-safe data structure used for passing data from one thread to
// another in a buffered manner.
template <typename T>
class Stream {
 public:
  Stream() : is_closed_(false), data_(1000, -1) {}

  bool Put(std::unique_ptr<T>* value) {
    if (!is_closed_) {
      return data_.Store(value);
    }
    return false;
  }

  // Receives an element of type T from sender, blocks until element is received
  // or thread is closed. Returns None if stream is closed.
  bool Take(std::unique_ptr<T>* value) {
    if (!is_closed_) {
      while (!data_.Load(value)) {}
      return true;
    } else {
      return false;
    }
  }

  bool is_closed() { return is_closed_; }

  // Closes stream.
  void Close() { is_closed_ = true; }

 private:
  std::atomic<bool> is_closed_;
  ResizableRingBuffer<T> data_;
};

} // namespace utility

#endif // TURBO_SANTA_COMMON_UTILITY_STREAM_H_
