#ifndef TURBO_SANTA_COMMON_UTILITY_QUEUE_H_
#define TURBO_SANTA_COMMON_UTILITY_QUEUE_H_

#include <memory>
#include "glog/logging.h"

namespace utility {

template<typename T>
class QueueNode {
 public:
  QueueNode() {}
  QueueNode(T elem) : elem_(std::forward<T>(elem)) {}
  QueueNode(T elem, std::unique_ptr<QueueNode<T>> next) : 
      elem_(std::forward<T>(elem)),
      next_(std::forward<std::unique_ptr<QueueNode<T>>>(next)) {}

  T TakeValue() { return std::move(elem_); }

  std::unique_ptr<QueueNode<T>> TakeNext() { return std::move(next_); }

  void set_next(std::unique_ptr<QueueNode<T>> next) {
    next_ = std::forward<std::unique_ptr<QueueNode<T>>>(next);
  }

  QueueNode<T>* next() { return next_.get(); }

 private:
  T elem_;
  std::unique_ptr<QueueNode<T>> next_;
};

template<typename T>
class Queue {
 public:
  Queue() {}

  void Push(T elem) {
    if (head_) {
      std::unique_ptr<QueueNode<T>> queue_node 
          = std::unique_ptr<QueueNode<T>>(
              new QueueNode<T>(std::forward<T>(elem)));
      tail_->set_next(std::forward<std::unique_ptr<QueueNode<T>>>(queue_node));
      tail_ = tail_->next();
    } else {
      head_ = std::unique_ptr<QueueNode<T>>(
          new QueueNode<T>(std::forward<T>(elem)));
      tail_ = head_.get();
    }
    size_++;
  }

  T Pop() {
    if (size_ <= 0) {
      LOG(FATAL) << "Pop was called on empty queue.";
    }
    T result = head_->TakeValue();
    head_ = head_->TakeNext();
    size_--;
    return std::move(result);
  }

  bool is_empty() { return size_ <= 0; }

 private:
  size_t size_ = 0;
  std::unique_ptr<QueueNode<T>> head_;
  QueueNode<T>* tail_;
};

} // namespace utility

#endif // TURBO_SANTA_COMMON_UTILITY_QUEUE_H_
