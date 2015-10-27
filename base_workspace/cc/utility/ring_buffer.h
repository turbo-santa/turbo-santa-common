#ifndef TURBO_SANTA_COMMON_UTILITY_RING_BUFFER_H_
#define TURBO_SANTA_COMMON_UTILITY_RING_BUFFER_H_

#include <atomic>
#include <cstdint>
#include <memory>
#include <vector>

namespace utility {

template<typename T>
class RingBuffer {
 public:
  RingBuffer(size_t size) : atomic_write_index_(-1), atomic_read_index_(-1), buffer_(size) {}

  bool Store(std::unique_ptr<T>* value) {
    int64_t write_index = atomic_write_index_.load(std::memory_order_acquire) + 1;
    int64_t read_index = atomic_read_index_.load(std::memory_order_acquire);
    int64_t size = buffer_.size();

    if (write_index - read_index < size) {
      buffer_[write_index % size] = std::move(*value);
      atomic_write_index_.store(write_index, std::memory_order_release);
      return true;
    } else {
      return false;
    }
  }

  bool RollbackStore(std::unique_ptr<T>* return_value) {
    int64_t write_index = atomic_write_index_.fetch_add(-1, std::memory_order_acquire);
    int64_t read_index = atomic_read_index_.load(std::memory_order_acquire);
    int64_t size = buffer_.size();

    if (write_index > read_index) {
      *return_value = std::move(buffer_[write_index % size]);
      return true;
    } else {
      atomic_write_index_.store(write_index + 1, std::memory_order_relaxed);
      return false;
    }
  }

  bool RollbackStoreToClear(std::unique_ptr<T>* return_value) {
    int64_t write_index = atomic_write_index_.fetch_add(1, std::memory_order_acquire);
    int64_t read_index = atomic_read_index_.load(std::memory_order_acquire);
    int64_t size = buffer_.size();

    if (write_index >= read_index) {
      *return_value = std::move(buffer_[write_index % size]);
      return true;
    } else {
      atomic_write_index_.store(write_index - 1, std::memory_order_relaxed);
      return false;
    }
  }

  bool Load(std::unique_ptr<T>* return_value) {
    int64_t write_index = atomic_write_index_.load(std::memory_order_acquire);
    int64_t read_index = atomic_read_index_.load(std::memory_order_acquire) + 1;
    int64_t size = buffer_.size();

    if (read_index <= write_index) {
      *return_value = std::move(buffer_[read_index % size]);
      atomic_read_index_.store(read_index, std::memory_order_release);
      return true;
    } else {
      return false;
    }
  }

  size_t size() { return buffer_.size(); }

 private:
  std::atomic<int64_t> atomic_write_index_;
  std::atomic<int64_t> atomic_read_index_;
  std::vector<std::unique_ptr<T>> buffer_;
};

} // namespace utility

#endif // TURBO_SANTA_COMMON_UTILITY_RING_BUFFER_H_
