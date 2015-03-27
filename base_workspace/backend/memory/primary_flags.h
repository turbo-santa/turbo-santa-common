#ifndef TURBO_SANTA_COMMON_BACK_END_MEMORY_PRIMARY_FLAGS_H_
#define TURBO_SANTA_COMMON_BACK_END_MEMORY_PRIMARY_FLAGS_H_

#include "backend/config.h"

#include "backend/memory/interrupt_flag.h"
#include "backend/memory/module.h"

namespace back_end {
namespace memory {

class PrimaryFlags : public Module {
 public:
  void Init() {
    add_flag(&interrupt_enable_);
    add_flag(&interrupt_flag_);
  }

  InterruptEnable* interrupt_enable() { return &interrupt_enable_; }
  InterruptFlag* interrupt_flag() { return &interrupt_flag_; }
  
 private:
  InterruptEnable interrupt_enable_;
  InterruptFlag interrupt_flag_;
};

} // namespace memory
} // namespace back_end

#endif // TURBO_SANTA_COMMON_BACK_END_MEMORY_PRIMARY_FLAGS_H_
