#ifndef TURBO_SANTA_COMMON_BACK_END_SOUND_SOUND_CONTROLLER_H_
#define TURBO_SANTA_COMMON_BACK_END_SOUND_SOUND_CONTROLLER_H_

#include <vector>
#include "cc/backend/memory/memory_segment.h"
#include "cc/backend/memory/module.h"

namespace backend {
namespace sound {

class WavePatternSegment : public memory::ContiguousMemorySegment {
  public:
    WavePatternSegment() :
      data_(upper_address_bound() - lower_address_bound() + 1)  {};

  unsigned char Read(unsigned short address) override {
    return data_[address - lower_address_bound()];
  };

  void Write(unsigned short address, unsigned char value) override {
    data_[address - lower_address_bound()] = value;
  };

  protected:
    uint16_t lower_address_bound() override { return 0xff30; }
    uint16_t upper_address_bound() override { return 0xff3f; }

  private:
    std::vector<uint8_t> data_;
};

class SoundController : public memory::Module {
  public:
    void Init();

    void Tick(uint32_t ticks);

  private:
    memory::Flag sound_ch_1_sweep_ = memory::Flag(0xff10);
    memory::Flag sound_ch_1_sound_length_ = memory::Flag(0xff11);
    memory::Flag sound_ch_1_volume_ = memory::Flag(0xff12);
    memory::Flag sound_ch_1_freq_lo_ = memory::Flag(0xff13);
    memory::Flag sound_ch_1_freq_hi_ = memory::Flag(0xff14);
    memory::Flag sound_ch_2_sound_length_ = memory::Flag(0xff16);
    memory::Flag sound_ch_2_volume_ = memory::Flag(0xff17);
    memory::Flag sound_ch_2_freq_lo_ = memory::Flag(0xff18);
    memory::Flag sound_ch_2_freq_hi_ = memory::Flag(0xff19);
    memory::Flag sound_ch_3_on_off_ = memory::Flag(0xff1A);
    memory::Flag sound_ch_3_sound_length_ = memory::Flag(0xff1B);
    memory::Flag sound_ch_3_output_level_ = memory::Flag(0xff1C);
    memory::Flag sound_ch_3_freq_lo_ = memory::Flag(0xff1D);
    memory::Flag sound_ch_3_freq_hi_ = memory::Flag(0xff1E);
    memory::Flag sound_ch_4_sound_length_ = memory::Flag(0xff20);
    memory::Flag sound_ch_4_volume_ = memory::Flag(0xff21);
    memory::Flag sound_ch_4_poly_counter_ = memory::Flag(0xff22);
    memory::Flag sound_ch_4_counter_ = memory::Flag(0xff23);
    memory::Flag sound_chan_ctrl_ = memory::Flag(0xff24);
    memory::Flag sound_output_terminal_ = memory::Flag(0xff25);
    memory::Flag sound_on_off_ = memory::Flag(0xff26);
    WavePatternSegment wave_pattern_ram_;
};

} // namespace sound
} // namespace backend

#endif // #ifndef TURBO_SANTA_COMMON_BACK_END_SOUND_SOUND_CONTROLLER_H_