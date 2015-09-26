#include "sound_controller.h"

namespace backend {
namespace sound {

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
}

void SoundController::Tick(uint32_t ticks) {

}

} // namespace sound
} // namespace backend