#ifndef TURBO_SANTA_COMMON_BACK_END_SOUND_SOUND_PLAYER_H_
#define TURBO_SANTA_COMMON_BACK_END_SOUND_SOUND_PLAYER_H_

#include <cstdint>

namespace backend {
namespace sound {

class SoundPlayer {
 public:
  virtual void SubmitAudioSample(int8_t* data, int length) = 0;
};

} // namespace sound
} // namespace backend


#endif // TURBO_SANTA_COMMON_BACK_END_SOUND_SOUND_PLAYER_H_