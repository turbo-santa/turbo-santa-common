#include "cc/backend/sound/tone_and_sweep_channel.h"
#include "cc/backend/sound/sound_utils.h"
#include "glog/logging.h"

#define _USE_MATH_DEFINES
#include <math.h>

namespace backend {
namespace sound {

using std::vector;

vector<int8_t> generateSquareWave(uint16_t frequency, double sweep_time,
                                  double duty_cycle);

double frequency_convert(uint16_t frequency) {
//  LOG(WARNING) << "converting: " << frequency;
  return 131072.0 / (2048.0 - frequency);
}

vector<int8_t> ToneAndSweep::sound_data() {
  frequency_ = frequency_convert(frequency_lo_->flag() | (static_cast<uint16_t>(frequency_hi_->value()) << 8));

//  LOG(WARNING) << "frequency lo: " << frequency_lo_->flag() + 0 << " frequency hi: " << frequency_hi_->value() + 0 << " frequency: " << frequency_;
  return generateSquareWave(frequency_, 0, 0);
}

bool ToneAndSweep::has_data() {
  return frequency_lo_->flag() || frequency_hi_->value();
}

/*
 * Produces a byte array representing a single channel square wave with the
 * specified frequency, sweep, and duty cycle.
 * frequency in hertz
 * sweep_time in ms
 * duty_cycle
 */
vector<int8_t> generateSquareWave(uint16_t frequency, double sweep_time, 
                                  double duty_cycle) {
  // TODO: figure out what the length should be
  uint32_t length = static_cast<uint32_t>(0.01 * kSampleRate);
  vector<int8_t> sound_bytes(length);
  double period = static_cast<double>(kSampleRate) / 1024;

//  LOG(WARNING) << "frequency: " << frequency << " sample rate: " << kSampleRate << " period: " << period;
  // TODO: implement duty cycle and sweep
  for (uint32_t i = 0; i < length; i++) {
    double angle = (2.0 * M_PI * i) / period;
    sound_bytes[i] = sgn(sin(angle)) * 127;
  }
  return sound_bytes;
}
/*
vector<int8_t> frequencyToBytes(uint16_t frequency1, double sampleLength) {
   int SAMPLE_RATE = 44100;
   int length = sampleLength * SAMPLE_RATE;
   std::vector<int8_t> soundBytes(length);

   for (int i = 0; i < length; i += 2) {
     double period1 = (double) SAMPLE_RATE / frequency1;
     double angle1 = (2.0 * PI * i) / period1;
     soundBytes[i] = (int8_t) (sin(angle1) * 127);
   }
   return soundBytes;
}
*/
} // namespace sound
} // namespace backend
