#ifndef TURBO_SANTA_COMMON_BACK_END_MEMORY_TIMER_TIMER_MODULE_H_
#define TURBO_SANTA_COMMON_BACK_END_MEMORY_TIMER_TIMER_MODULE_H_

#include <cstdint>

#include "cc/backend/memory/flags.h"
#include "cc/backend/memory/interrupt/interrupt_flag.h"
#include "cc/backend/memory/module.h"
#include "cc/backend/memory/timer/divider_flag.h"
#include "submodules/glog/src/glog/logging.h"

namespace backend {
namespace memory {
namespace timer {

class TimerControlFlag : public Flag {
 public:
  static const int k4096Hz = 4096;
  static const int k262144Hz = 262144;
  static const int k65536Hz = 65536;
  static const int k16384Hz = 16384;

  TimerControlFlag() : Flag(0xff07) {}

  void Write(uint16_t, uint8_t value) override { set_flag(value & 0b00000111); }

  int timer_speed() {
    switch (flag() & 0b00000011) {
      case 0:
        return k4096Hz;
      case 1:
        return k262144Hz;
      case 2:
        return k65536Hz;
      case 4:
        return k16384Hz;
      default:
        LOG(FATAL) << "Invalid timer speed.";
    }
  }

  bool is_timer_running() {
    return bit(2);
  }
};

class TimerModule : public Module {
 public:
  void Init(InterruptFlag* interrupt_flag) {
    interrupt_flag_ = interrupt_flag;
    add_flag(&divider_flag_);
    add_flag(&timer_counter_);
    add_flag(&timer_modulo_);
    add_flag(&timer_control_);
  }

  void Tick(int ticks) {
    divider_flag_.Tick(ticks);
    if (!timer_control_.is_timer_running()) {
      return;
    }

    ticks_ += ticks;
    if (ticks_ >= ticks_to_increment()) {
      if (timer_counter_.flag() == 0xff) {
        timer_counter_.set_flag(timer_modulo_.flag());
        interrupt_flag_->set_timer(true);
      } else {
        timer_counter_.set_flag(timer_counter_.flag() + 1);
      }
    }
  }

 private:
  int ticks_ = 0;
  InterruptFlag* interrupt_flag_;
  Flag timer_counter_ = Flag(0xff05);
  Flag timer_modulo_ = Flag(0xff06);
  TimerControlFlag timer_control_;
  DividerFlag divider_flag_;

  int ticks_to_increment() { return 4194304 / timer_control_.timer_speed(); }
};

} // namespace timer
} // namespace memory
} // namespace backend

#endif // TURBO_SANTA_COMMON_BACK_END_MEMORY_TIMER_TIMER_MODULE_H_
