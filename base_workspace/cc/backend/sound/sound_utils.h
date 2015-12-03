#ifndef TURBO_SANTA_COMMON_BACK_END_SOUND_SOUND_UTILS_H_
#define TURBO_SANTA_COMMON_BACK_END_SOUND_SOUND_UTILS_H_

namespace backend {
namespace sound {

static const uint16_t kSampleRate = 44100;

int8_t sgn(double val) {
  return (0 < val) - (val < 0);
}

} // sound
} // backend

#endif // TURBO_SANTA_COMMON_BACK_END_SOUND_SOUND_UTILS_H_
