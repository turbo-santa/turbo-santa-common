#ifndef TURBO_SANTA_COMMON_BACK_END_MEMORY_PRIMARY_FLAGS_H_
#define TURBO_SANTA_COMMON_BACK_END_MEMORY_PRIMARY_FLAGS_H_

#include "cc/backend/memory/interrupt/interrupt_flag.h"
#include "cc/backend/memory/module.h"
#include "submodules/glog/src/glog/logging.h"

namespace backend {
namespace memory {

class PrimaryFlags : public Module {
 public:
  void Init() {
    add_flag(&interrupt_enable_);
    add_flag(&interrupt_flag_);
    LOG(INFO) << "Interrupt Enable address = 0x" << std::hex << interrupt_enable_.address();
  }

  InterruptEnable* interrupt_enable() { return &interrupt_enable_; }
  InterruptFlag* interrupt_flag() { return &interrupt_flag_; }
  
 private:
  InterruptEnable interrupt_enable_;
  InterruptFlag interrupt_flag_;
};

} // namespace memory
} // namespace backend

#endif // TURBO_SANTA_COMMON_BACK_END_MEMORY_PRIMARY_FLAGS_H_
