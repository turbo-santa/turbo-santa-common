#include "sound_controller.h"

#include <cstdint>
#include <math.h>

namespace backend {
namespace sound {

std::vector<int8_t> frequencyToBytes(int, int, double);

void SoundController::Init() {
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
  add_flag(&sound_ch_4_sound_length_);
  add_flag(&sound_ch_4_volume_);
  add_flag(&sound_ch_4_poly_counter_);
  add_flag(&sound_ch_4_counter_);
  add_flag(&sound_chan_ctrl_);
  add_flag(&sound_output_terminal_);
  add_flag(&sound_on_off_);
  add_memory_segment(&wave_pattern_ram_);
  ticks_ = 1;
}

void SoundController::Tick(uint32_t ticks) {
  if (ticks_) {
    std::vector<int8_t> sound = frequencyToBytes(440, 2440, 10);
    sound_player_->SubmitAudioSample(sound.data(), sound.size());
    ticks_ = 0;
  }
}

std::vector<int8_t> frequencyToBytes(int frequency1, int frequency2, double sampleLength) {
   double PI = 3.141592653589793;
   int SAMPLE_RATE = 44100;
   int length = sampleLength * SAMPLE_RATE;
   std::vector<int8_t> soundBytes(length);

   for (int i = 0; i < length; i += 2) {
     double period1 = (double) SAMPLE_RATE / frequency1;
     double angle1 = (2.0 * PI * i) / period1;
     soundBytes[i] = (int8_t) (sin(angle1) * 127);
     double period2 = (double) SAMPLE_RATE / frequency2;
     double angle2 = (2.0 * PI * i) / period2;
     soundBytes[i + 1] = (int8_t) (sin(angle2) * 127);
   }
   return soundBytes;
}

} // namespace sound
} // namespace backend