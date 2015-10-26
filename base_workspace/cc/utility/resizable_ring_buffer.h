#ifndef TURBO_SANTA_COMMON_UTILITY_RESIZABLE_RING_BUFFER_H_
#define TURBO_SANTA_COMMON_UTILITY_RESIZABLE_RING_BUFFER_H_

#include <atomic>
#include <memory>
#include "cc/utility/ring_buffer.h"

namespace utility {

template<typename T>
class ResizableRingBuffer {
 public:
  ResizableRingBuffer(size_t starting_size, size_t max_size) :
      loading_(false),
      switching_buffers_(false),
      max_size_(max_size), 
      buffer_(new RingBuffer<T>(starting_size)) {}

  bool Store(std::unique_ptr<T>* value) {
    if (buffer_->Store(value)) {
      return true;
    } else if (Resize()) {
      buffer_->Store(value);
      return true;
    } else {
      return false;
    }
  }

  bool Load(std::unique_ptr<T>* return_value) {
    loading_.store(true, std::memory_order_acquire); // Let potential resizes know not to clear buffer.
    if (!switching_buffers_.load(std::memory_order_acquire)) { // Need to prevent load if buffer is switching.
      bool result = buffer_->Load(return_value);
      loading_.store(false, std::memory_order_relaxed);
      return result;
    } else {
      loading_.store(false, std::memory_order_relaxed);
      return false;
    }
  }

 private:
  std::atomic<bool> loading_;
  std::atomic<bool> switching_buffers_;
  size_t max_size_;
  std::unique_ptr<RingBuffer<T>> buffer_;

  bool Resize() {
    size_t new_size = buffer_.size() * 2;
    if (max_size_ > 0) {
      if (buffer_.size() < max_size_) {
        new_size = max(new_size, max_size_);
      } else {
        // Can't expand buffer.
        return false;
      }
    }

    std::unique_ptr<RingBuffer<T>> new_buffer(new RingBuffer<T>(new_size));
    std::unique_ptr<T> current_value;

    // Rollback elements from old buffer_ and store to be replayed on the
    // new buffer.
    std::vector<std::unique_ptr<T>> temp_stack;
    while (buffer_->RollbackStore(&current_value)) {
      temp_stack.push_back(std::move(current_value));
    }

    switching_buffers_.store(true, std::memory_order_acquire); // LOCK buffer_.
    if (!loading_.load(std::memory_order_acquire)) {
      // If the reader is currently not reading, make sure the buffer_ is empty.
      if (buffer_->RollbackStoreToClear(&current_value)) {
        temp_stack.push_back(std::move(current_value));
      }
    } else {
      // Reader is reading now so we need to wait until it is done.
      while (loading_.load(std::memory_order_acquire)) {}
    }
    // Copy rolled back data back to new buffer.
    while (temp_stack.size() > 0) {
      current_value = temp_stack.pop_back();
      new_buffer.Store(&current_value);
    }
    buffer_.reset(std::move(new_buffer));
    switching_buffers_.store(false, std::memory_order_release); // UNLOCK buffer_;

    return true;
  }
};

} // namespace utility

#endif // TURBO_SANTA_COMMON_UTILITY_RESIZABLE_RING_BUFFER_H_
