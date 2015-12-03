#ifndef TURBO_SANTA_COMMON_BACK_END_SOUND_SOUND_CHANNEL_H_
#define TURBO_SANTA_COMMON_BACK_END_SOUND_SOUND_CHANNEL_H_

#include <vector>

namespace backend {
namespace sound {

class SoundChannel {
  public:
    virtual std::vector<int8_t> sound_data() = 0;
};

} // namespace sound
} // namespace backend
#endif // TURBO_SANTA_COMMON_BACK_END_SOUND_SOUND_CHANNEL_H_