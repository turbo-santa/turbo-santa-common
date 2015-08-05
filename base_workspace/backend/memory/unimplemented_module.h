#ifndef TURBO_SANTA_COMMON_BACK_END_MEMORY_UNIMPLEMENTED_MODULE_H_
#define TURBO_SANTA_COMMON_BACK_END_MEMORY_UNIMPLEMENTED_MODULE_H_

#include <string>
#include "backend/memory/memory_segment.h"
#include "backend/memory/module.h"
#include "backend/memory/flags.h"
#include "submodules/glog/src/glog/logging.h"

namespace back_end {
namespace memory {

class UnimplementedFlag : public Flag {
 public:
  UnimplementedFlag(unsigned short address, std::string name) 
      : Flag(address), name_(name) {}

  virtual unsigned char Read(unsigned short) {
    LOG(ERROR) << "Attempted to read unimplemented flag: " << name_;
    return 0;
  }
  virtual void Write(unsigned short, unsigned char value) {
    unsigned short value_short = value;
    LOG(ERROR) << "Attempted to write unimplemented flag: " << name_ << " = 0x" << std::hex << value_short;
  }

  virtual unsigned char flag() {
    LOG(FATAL) << "Attempted to access unimplemented flag: " << name_;
  }

  virtual void set_flag(unsigned char) {
    LOG(FATAL) << "Attempted to access unimplemented flag: " << name_;
  }

 protected:
  std::string name_;
};

class UnimplementedJoypadFlag : public UnimplementedFlag {
 public:
  UnimplementedJoypadFlag(uint16_t address) : UnimplementedFlag(address, "Joypad") {}

  unsigned char Read(unsigned short) override {
    LOG(WARNING) << "Attempted to read unimplemented flag: " << name_;
    return 0b00001111;
  }
};

class UnimplementedModule : public Module {
 public:
  void Init() {
    add_flag(&sound_ch_1_sweep_);
    add_flag(&sound_ch_1_sound_length_);
    add_flag(&sound_ch_1_volume_); 
    add_flag(&sound_ch_1_freq_lo_);
    add_flag(&sound_ch_1_freq_hi_);
    add_flag(&sound_ch_2_sound_length_);
    add_flag(&sound_ch_2_volume_);
    add_flag(&sound_ch_2_freq_lo_);
    add_flag(&sound_ch_2_freq_hi_);
    add_flag(&sound_ch_3_on_off_);
    add_flag(&sound_ch_3_sound_length_);
    add_flag(&sound_ch_3_output_level_);
    add_flag(&sound_ch_3_freq_lo_);
    add_flag(&sound_ch_3_freq_hi_);
    add_flag(&sound_ch_3_wave_pattern_0_);
    add_flag(&sound_ch_3_wave_pattern_1_);
    add_flag(&sound_ch_3_wave_pattern_2_);
    add_flag(&sound_ch_3_wave_pattern_3_);
    add_flag(&sound_ch_3_wave_pattern_4_);
    add_flag(&sound_ch_3_wave_pattern_5_);
    add_flag(&sound_ch_3_wave_pattern_6_);
    add_flag(&sound_ch_3_wave_pattern_7_);
    add_flag(&sound_ch_3_wave_pattern_8_);
    add_flag(&sound_ch_3_wave_pattern_9_);
    add_flag(&sound_ch_3_wave_pattern_a_);
    add_flag(&sound_ch_3_wave_pattern_b_);
    add_flag(&sound_ch_3_wave_pattern_c_);
    add_flag(&sound_ch_3_wave_pattern_d_);
    add_flag(&sound_ch_3_wave_pattern_e_);
    add_flag(&sound_ch_3_wave_pattern_f_);
    add_flag(&sound_ch_4_sound_length_);
    add_flag(&sound_ch_4_volume_);
    add_flag(&sound_ch_4_poly_counter_);
    add_flag(&sound_ch_4_counter_);
    add_flag(&sound_chan_ctrl_);     
    add_flag(&sound_output_terminal_);
    add_flag(&sound_on_off_);
    add_flag(&serial_data_transfer_);
    add_flag(&serial_control_);
  }

 private:
//  UnimplementedFlag dma_transfer_ =              UnimplementedFlag(0xff46, "DMA Transfer");
  UnimplementedFlag sound_ch_1_sweep_ =          UnimplementedFlag(0xff10, "Sound - CH 1 - Sweep");
  UnimplementedFlag sound_ch_1_sound_length_ =   UnimplementedFlag(0xff11, "Sound - CH 1 - Length/Wave Duty");
  UnimplementedFlag sound_ch_1_volume_ =         UnimplementedFlag(0xff12, "Sound - CH 1 - Volume");
  UnimplementedFlag sound_ch_1_freq_lo_ =        UnimplementedFlag(0xff13, "Sound - CH 1 - Frequency Low");
  UnimplementedFlag sound_ch_1_freq_hi_ =        UnimplementedFlag(0xff14, "Sound - CH 1 - Frequency High");
  UnimplementedFlag sound_ch_2_sound_length_ =   UnimplementedFlag(0xff16, "Sound - CH 2 - Length/Wave Duty");
  UnimplementedFlag sound_ch_2_volume_ =         UnimplementedFlag(0xff17, "Sound - CH 2 - Volume");
  UnimplementedFlag sound_ch_2_freq_lo_ =        UnimplementedFlag(0xff18, "Sound - CH 2 - Frequency Low");
  UnimplementedFlag sound_ch_2_freq_hi_ =        UnimplementedFlag(0xff19, "Sound - CH 2 - Frequency High");
  UnimplementedFlag sound_ch_3_on_off_ =         UnimplementedFlag(0xff1a, "Sound - CH 3 - ON/OFF");
  UnimplementedFlag sound_ch_3_sound_length_ =   UnimplementedFlag(0xff1b, "Sound - CH 3 - Length/Wave Duty");
  UnimplementedFlag sound_ch_3_output_level_ =   UnimplementedFlag(0xff1c, "Sound - CH 3 - Volume");
  UnimplementedFlag sound_ch_3_freq_lo_ =        UnimplementedFlag(0xff1d, "Sound - CH 3 - Frequency Low");
  UnimplementedFlag sound_ch_3_freq_hi_ =        UnimplementedFlag(0xff1e, "Sound - CH 3 - Frequency High");
  UnimplementedFlag sound_ch_3_wave_pattern_0_ = UnimplementedFlag(0xff30, "Sound - CH 3 - Wave Pattern 0");
  UnimplementedFlag sound_ch_3_wave_pattern_1_ = UnimplementedFlag(0xff31, "Sound - CH 3 - Wave Pattern 1");
  UnimplementedFlag sound_ch_3_wave_pattern_2_ = UnimplementedFlag(0xff32, "Sound - CH 3 - Wave Pattern 2");
  UnimplementedFlag sound_ch_3_wave_pattern_3_ = UnimplementedFlag(0xff33, "Sound - CH 3 - Wave Pattern 3");
  UnimplementedFlag sound_ch_3_wave_pattern_4_ = UnimplementedFlag(0xff34, "Sound - CH 3 - Wave Pattern 4");
  UnimplementedFlag sound_ch_3_wave_pattern_5_ = UnimplementedFlag(0xff35, "Sound - CH 3 - Wave Pattern 5");
  UnimplementedFlag sound_ch_3_wave_pattern_6_ = UnimplementedFlag(0xff36, "Sound - CH 3 - Wave Pattern 6");
  UnimplementedFlag sound_ch_3_wave_pattern_7_ = UnimplementedFlag(0xff37, "Sound - CH 3 - Wave Pattern 7");
  UnimplementedFlag sound_ch_3_wave_pattern_8_ = UnimplementedFlag(0xff38, "Sound - CH 3 - Wave Pattern 8");
  UnimplementedFlag sound_ch_3_wave_pattern_9_ = UnimplementedFlag(0xff39, "Sound - CH 3 - Wave Pattern 9");
  UnimplementedFlag sound_ch_3_wave_pattern_a_ = UnimplementedFlag(0xff3a, "Sound - CH 3 - Wave Pattern a");
  UnimplementedFlag sound_ch_3_wave_pattern_b_ = UnimplementedFlag(0xff3b, "Sound - CH 3 - Wave Pattern b");
  UnimplementedFlag sound_ch_3_wave_pattern_c_ = UnimplementedFlag(0xff3c, "Sound - CH 3 - Wave Pattern c");
  UnimplementedFlag sound_ch_3_wave_pattern_d_ = UnimplementedFlag(0xff3d, "Sound - CH 3 - Wave Pattern d");
  UnimplementedFlag sound_ch_3_wave_pattern_e_ = UnimplementedFlag(0xff3e, "Sound - CH 3 - Wave Pattern e");
  UnimplementedFlag sound_ch_3_wave_pattern_f_ = UnimplementedFlag(0xff3f, "Sound - CH 3 - Wave Pattern f");
  UnimplementedFlag sound_ch_4_sound_length_ =   UnimplementedFlag(0xff20, "Sound - CH 4 - Length");
  UnimplementedFlag sound_ch_4_volume_ =         UnimplementedFlag(0xff21, "Sound - CH 4 - Volume");
  UnimplementedFlag sound_ch_4_poly_counter_ =   UnimplementedFlag(0xff22, "Sound - CH 4 - Polynomial Counter");
  UnimplementedFlag sound_ch_4_counter_ =        UnimplementedFlag(0xff23, "Sound - CH 4 - Counter");
  UnimplementedFlag sound_chan_ctrl_ =           UnimplementedFlag(0xff24, "Sound - Channel Control");
  UnimplementedFlag sound_output_terminal_ =     UnimplementedFlag(0xff25, "Sound - Output Terminal");
  UnimplementedFlag sound_on_off_ =              UnimplementedFlag(0xff26, "Sound - ON/OFF");
  // UnimplementedJoypadFlag joypad_ =              UnimplementedJoypadFlag(0xff00);
  UnimplementedFlag serial_data_transfer_ =      UnimplementedFlag(0xff01, "Serial - Data Transfer");
  UnimplementedFlag serial_control_ =            UnimplementedFlag(0xff02, "Serial - Control");
  // UnimplementedFlag timer_divider_ =             UnimplementedFlag(0xff04, "Timer - Divider");
  // UnimplementedFlag timer_counter_ =             UnimplementedFlag(0xff05, "Timer - Counter");
  // UnimplementedFlag timer_modulo_ =              UnimplementedFlag(0xff06, "Timer - Modulo");
  // UnimplementedFlag timer_control_ =             UnimplementedFlag(0xff07, "Timer - Control");
};

} // namespace memory
} // namespace back_end

#endif // TURBO_SANTA_COMMON_BACK_END_MEMORY_UNIMPLEMENTED_MODULE_H_
