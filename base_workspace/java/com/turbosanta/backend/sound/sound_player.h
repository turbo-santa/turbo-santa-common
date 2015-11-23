#ifndef JAVA_COM_TURBOSANTA_BACKEND_SOUND_SOUND_PLAYER_H_
#define JAVA_COM_TURBOSANTA_BACKEND_SOUND_SOUND_PLAYER_H_

#include <cstdint>
#include <jni.h>
#include "cc/backend/sound/sound_player.h"

namespace java_com_turbosanta_backend {
namespace sound {

class SoundPlayer : public backend::sound::SoundPlayer {
 public:
  SoundPlayer(JavaVM* jvm, JNIEnv* env, jobject native_audio);

  void SubmitAudioSample(int8_t* data, int length) {
    JNIEnv* env;
    jvm_->AttachCurrentThread((void**) &env, nullptr);

    jbyteArray byte_array;
    byte_array = env->NewByteArray(length);
    env->SetByteArrayRegion(byte_array, 0, length, data);
    env->CallVoidMethod(native_audio_, submitAudioSampleMID_, byte_array);
  }

  private:
   JavaVM* jvm_;
   jobject native_audio_;
   jmethodID submitAudioSampleMID_;
};

} // namespace sound
} // namespace java_com_turbosanta_backend

#endif // JAVA_COM_TURBOSANTA_BACKEND_SOUND_SOUND_PLAYER_H_