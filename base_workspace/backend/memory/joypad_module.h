#ifndef TURBO_SANTA_COMMON_BACK_END_MEMORY_JOYPAD_MODULE_H_
#define TURBO_SANTA_COMMON_BACK_END_MEMORY_JOYPAD_MODULE_H_

#include <cstdint>
#include <memory>
#include "backend/memory/flags.h"
#include "backend/memory/interrupt_flag.h"
#include "backend/memory/module.h"
#include "submodules/glog/src/glog/logging.h"

namespace back_end {
namespace memory {

class JoypadFlag : public Flag {
 public:
  JoypadFlag(InterruptFlag* interrupt_flag) : 
      Flag(0xff00), 
      interrupt_flag_(interrupt_flag) {}

  unsigned char Read(unsigned short) override {
    uint8_t button_keys_selected = (!is_button_keys_selected_) << 5;
    uint8_t direction_keys_selected = (!is_direction_keys_selected_) << 4;
    if (is_button_keys_selected_ && is_direction_keys_selected_) {
      LOG(ERROR) << "Both button matrices selected durring read.";
      return button_keys_selected 
          | direction_keys_selected 
          | button_keys_ 
          | direction_keys_;
    } else if (is_button_keys_selected_) {
      if (~(0b11110000 | button_keys_)) {
        LOG(INFO) << "Reading set button keys.";
      } else {
        LOG(INFO) << "Reading unset button keys.";
      }
      return button_keys_selected | direction_keys_selected | button_keys_;
    } else if (is_direction_keys_selected_) {
      if (~(0b11110000 | direction_keys_)) {
        LOG(INFO) << "Reading set direction keys.";
      } else {
        LOG(INFO) << "Reading unset direction keys.";
      }
      return button_keys_selected | direction_keys_selected | direction_keys_;
    } else {
      LOG(ERROR) << "Neither button matrices selected durring read.";
      return button_keys_selected | direction_keys_selected;
    }
  }

  void Write(unsigned short, unsigned char value) override {
    is_button_keys_selected_ = !(value & 0b00100000);
    is_direction_keys_selected_ = !(value & 0b00010000);
  }

  void set_down() { set_direction_keys(3); }
  void set_up() { set_direction_keys(2); }
  void set_left() { set_direction_keys(1); }
  void set_right() { set_direction_keys(0); }
  void set_start() { set_button_keys(3); }
  void set_select() { set_button_keys(2); }
  void set_b() { set_button_keys(1); }
  void set_a() { set_button_keys(0); }
  void clear() { button_keys_ = 0b00001111; direction_keys_ = 0b00001111; }

  void DecrementClearCounter(int amount) {
    if (clear_counter_ <= 0) {
      return;
    }

    clear_counter_ -= amount;
    if (clear_counter_ <= 0) {
      LOG(INFO) << "Clearing joypad input.";
      clear();
    }
  }

 private:
  static const int kClearCounterStart = 400000;
  bool is_button_keys_selected_ = false;
  bool is_direction_keys_selected_ = false;
  uint8_t button_keys_ = 0b00001111;
  uint8_t direction_keys_ = 0b00001111;
  int clear_counter_ = 0;
  InterruptFlag* interrupt_flag_;

  void set_button_keys(int value) {
    LOG(INFO) << "Setting joypad input.";
    button_keys_ &= ~(1 << value);
    clear_counter_ = kClearCounterStart;
    interrupt_flag_->set_joypad(true);
  }

  void set_direction_keys(int value) {
    LOG(INFO) << "Setting joypad input.";
    direction_keys_ &= ~(1 << value);
    clear_counter_ = kClearCounterStart;
    interrupt_flag_->set_joypad(true);
  }
};

class JoypadModule : public Module {
 public:
  void Init(InterruptFlag* interrupt_flag) {
    joypad_flag_ = std::unique_ptr<JoypadFlag>(new JoypadFlag(interrupt_flag));
    add_flag(joypad_flag_.get());
  }

  void Tick(unsigned int number_of_cycles) {
    joypad_flag_->DecrementClearCounter(number_of_cycles);
  }

  JoypadFlag* joypad_flag() { return joypad_flag_.get(); }

 private:
  std::unique_ptr<JoypadFlag> joypad_flag_;
};

} // namespace memory
} // namespace back_end
#endif // TURBO_SANTA_COMMON_BACK_END_MEMORY_JOYPAD_MODULE_H_
