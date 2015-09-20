#ifndef TURBO_SANTA_COMMON_BACK_END_DEBUG_MEMORY_PROFILER_MEMORY_ACCESS_H_
#define TURBO_SANTA_COMMON_BACK_END_DEBUG_MEMORY_PROFILER_MEMORY_ACCESS_H_

#include <cstdint>
#include <memory>

#include "cc/backend/debug/memory_profiler/memory_message_base.h"
#include "cc/backend/debug/publisher.h"

namespace backend {
namespace debug {
namespace memory_profiler {

class MemoryAccess : public MemoryMessageBase {
 public:
  enum Mode {
    READ,
    WRITE,
  };

  Mode mode() const { return mode_; }
  uint8_t old_value() const { return old_value_; }
  uint8_t new_value() const { return new_value_; }
  uint16_t address() const { return address_; }

  void set_mode(Mode mode) { mode_ = mode; }
  void set_old_value(uint8_t old_value) { old_value_ = old_value; }
  void set_new_value(uint8_t new_value) { new_value_ = new_value; }
  void set_address(uint16_t address) { address_ = address; }

  Type type() const override { return MEMORY_ACCESS; }

 private:
  Mode mode_;
  uint8_t old_value_;
  uint8_t new_value_;
  uint16_t address_;
};

std::unique_ptr<backend::debug::Message> MakeReadMessage(uint16_t address, 
                                                         uint8_t value);

std::unique_ptr<backend::debug::Message> MakeWriteMessage(uint16_t address, 
                                                          uint8_t old_value, 
                                                          uint8_t new_value);

} // namespace memory_profiler
} // namespace debug
} // namespace backend

#define PUBLISH_READ(address, value) \
    PUBLISH(backend::debug::memory_profiler::MakeReadMessage(address, value));

#define PUBLISH_WRITE(address, old_value, new_value) \
    PUBLISH(backend::debug::memory_profiler::MakeWriteMessage(address, \
                                                              old_value, \
                                                              new_value));

#endif // TURBO_SANTA_COMMON_BACK_END_DEBUG_MEMORY_PROFILER_MEMORY_ACCESS_H_
