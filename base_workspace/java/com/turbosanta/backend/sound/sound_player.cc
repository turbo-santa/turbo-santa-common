#include "java/com/turbosanta/backend/sound/sound_player.h"

namespace java_com_turbosanta_backend {
namespace sound {

SoundPlayer::SoundPlayer(JavaVM* jvm, JNIEnv* env, jobject native_audio) :
    jvm_(jvm), native_audio_(env->NewGlobalRef(native_audio)) {
      jclass klass = env->GetObjectClass(native_audio_);
      submitAudioSampleMID_ = env->GetMethodID(klass, "submitAudioChunk", "([B)V");
    }

} // namespace sound
} // namespace java_com_turbosanta_backend