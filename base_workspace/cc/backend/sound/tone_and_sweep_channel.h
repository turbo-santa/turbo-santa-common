#ifndef TURBO_SANTA_COMMON_BACK_END_SOUND_TONE_AND_SWEEP_CHANNEL_H_
#define TURBO_SANTA_COMMON_BACK_END_SOUND_TONE_AND_SWEEP_CHANNEL_H_

#include <vector>
#include "cc/backend/memory/flags.h"
#include "cc/backend/sound/sound_channel.h"
#include "cc/backend/sound/sound_flags.h"

namespace backend {
namespace sound {

class ToneAndSweep : public SoundChannel {
  public:
    ToneAndSweep(flags::Sweep* sweep, flags::SoundLength* sound_length,
                 flags::VolumeEnvelope* envelope, memory::Flag* frequency_lo,
                 flags::FrequencyHigh* frequency_hi) :
        sweep_(sweep),
        sound_length_(sound_length),
        volume_envelope_(envelope),
        frequency_lo_(frequency_lo),
        frequency_hi_(frequency_hi) {}
    virtual bool has_data();
    virtual std::vector<int8_t> sound_data();

  private:
    flags::Sweep* sweep_;
    flags::SoundLength* sound_length_;
    flags::VolumeEnvelope* volume_envelope_;
    memory::Flag* frequency_lo_;
    flags::FrequencyHigh* frequency_hi_;
    uint16_t frequency_;
};

} // namespace sound
} // namespace backend
#endif // TURBO_SANTA_COMMON_BACK_END_SOUND_TONE_AND_SWEEP_CHANNEL_H_