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

  void set_down(bool is_pushed) { set_direction_keys(3, is_pushed); }
  void set_up(bool is_pushed) { set_direction_keys(2, is_pushed); }
  void set_left(bool is_pushed) { set_direction_keys(1, is_pushed); }
  void set_right(bool is_pushed) { set_direction_keys(0, is_pushed); }
  void set_start(bool is_pushed) { set_button_keys(3, is_pushed); }
  void set_select(bool is_pushed) { set_button_keys(2, is_pushed); }
  void set_b(bool is_pushed) { set_button_keys(1, is_pushed); }
  void set_a(bool is_pushed) { set_button_keys(0, is_pushed); }
  void clear() { button_keys_ = 0b00001111; direction_keys_ = 0b00001111; }

 private:
  bool is_button_keys_selected_ = false;
  bool is_direction_keys_selected_ = false;
  uint8_t button_keys_ = 0b00001111;
  uint8_t direction_keys_ = 0b00001111;
  InterruptFlag* interrupt_flag_;

  void set_button_keys(int value) {
    LOG(INFO) << "Setting joypad input.";
    button_keys_ &= ~(1 << value);
    interrupt_flag_->set_joypad(true);
  }

  void unset_button_keys(int value) {
    LOG(INFO) << "Unsetting joypad input.";
    button_keys_ |= (1 << value);
    interrupt_flag_->set_joypad(true);
  }

  void set_button_keys(int value, bool is_set) {
    if (is_set) {
      set_button_keys(value);
    } else {
      unset_button_keys(value);
    }
  }

  void set_direction_keys(int value) {
    LOG(INFO) << "Setting joypad input.";
    direction_keys_ &= ~(1 << value);
    interrupt_flag_->set_joypad(true);
  }

  void unset_direction_keys(int value) {
    LOG(INFO) << "Unsetting joypad input.";
    direction_keys_ |= (1 << value);
    interrupt_flag_->set_joypad(true);
  }

  void set_direction_keys(int value, bool is_set) {
    if (is_set) {
      set_direction_keys(value);
    } else {
      unset_direction_keys(value);
    }
  }
};

class JoypadModule : public Module {
 public:
  void Init(InterruptFlag* interrupt_flag) {
    joypad_flag_ = std::unique_ptr<JoypadFlag>(new JoypadFlag(interrupt_flag));
    add_flag(joypad_flag_.get());
  }

  JoypadFlag* joypad_flag() { return joypad_flag_.get(); }

 private:
  std::unique_ptr<JoypadFlag> joypad_flag_;
};

} // namespace memory
} // namespace back_end
#endif // TURBO_SANTA_COMMON_BACK_END_MEMORY_JOYPAD_MODULE_H_
