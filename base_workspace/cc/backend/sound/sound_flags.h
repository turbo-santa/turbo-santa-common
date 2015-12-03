#ifndef TURBO_SANTA_COMMON_BACK_END_SOUND_SOUND_FLAGS_H_
#define TURBO_SANTA_COMMON_BACK_END_SOUND_SOUND_FLAGS_H_

#include "cc/backend/memory/flags.h"
#include "glog/logging.h"

namespace backend {
namespace sound {
namespace flags {

class Enable : public memory::Flag {
  public:
    Enable(unsigned short address) : memory::Flag(address) {}
    bool master_enabled() { return bit(7); }
    bool noise_enabled() { return bit(3); }
    bool wave_enabled() { return bit(2); }
    bool tone_enabled() { return bit(1); }
    bool tone_and_sweep_enabled() { return bit(0); }
    void set_noise(bool enabled) { set_bit(3, enabled); }
    void set_wave(bool enabled) { set_bit(2, enabled); }
    void set_tone(bool enabled) { set_bit(1, enabled); }
    void set_tone_and_sweep(bool enabled) { set_bit(0, enabled); }
};

class ChannelControl : public memory::Flag {
  public:
    ChannelControl(unsigned short address) : memory::Flag(address) {}
    uint8_t channel_level(uint8_t channel) {
      if (channel == 0) {
        return bit(0) | (bit(1) << 1) | (bit(2) << 2);
      } else if (channel == 1) {
        return bit(4) | (bit(5) << 1) | (bit(6) << 2);
      } else {
        LOG(FATAL) << "Invalid sound channel: " << channel + 0;
      }
    }
};

class OutputTerminal : public memory::Flag {
  public:
    OutputTerminal(unsigned short address) : memory::Flag(address) {}
    bool chan_1_term_1() { return bit(0); }
    bool chan_2_term_1() { return bit(1); }
    bool chan_3_term_1() { return bit(2); }
    bool chan_4_term_1() { return bit(3); }
    bool chan_1_term_2() { return bit(4); }
    bool chan_2_term_2() { return bit(5); }
    bool chan_3_term_2() { return bit(6); }
    bool chan_4_term_2() { return bit(7); }
};

// Tone and Sweep

class Sweep : public memory::Flag {
  public:
    Sweep(unsigned short address) : memory::Flag(address) {}
    uint8_t get_time() { return bit(4) | (bit(5) << 1) | (bit(6) << 2); }
    bool increasing() { return !bit(3); }
    uint8_t shift() { return bit(0) | (bit(1) << 1) | (bit(2) << 2); }
};

class SoundLength : public memory::Flag {
  public:
    SoundLength(unsigned short address) : memory::Flag(address) {}
    uint8_t duty() { return bit(6) | (bit(7) << 1); }
    uint8_t data() { return 0x1f & flag(); } // bits 0-5
};

class VolumeEnvelope : public memory::Flag {
  public:
    VolumeEnvelope(unsigned short address) : memory::Flag(address) {}
    uint8_t initial_volume() { return 0xf0 & flag(); } // bits 4-7
    bool increasing() { return bit(3); }
    uint8_t sweep_number() { return bit(0) | (bit(1) << 1) | (bit(2) << 2); }
};

class FrequencyHigh : public memory::Flag {
  public:
    FrequencyHigh(unsigned short address) : memory::Flag(address) {}
    bool initial() { return bit(7); }
    bool counter() { return bit(6); }
    uint8_t value() { return bit(0) | (bit(1) << 1) | (bit(2) << 2); }
};

} // namespace flags
} // namespace sound
} // namespace backend
#endif // TURBO_SANTA_COMMON_BACK_END_SOUND_SOUND_FLAGS_H_