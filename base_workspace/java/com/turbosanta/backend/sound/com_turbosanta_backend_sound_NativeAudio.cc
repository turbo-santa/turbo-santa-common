#include "java/com/turbosanta/backend/sound/com_turbosanta_backend_sound_NativeAudio.h"

#include "cc/backend/sound/sound_player.h"
#include "java/com/turbosanta/backend/sound/sound_player.h"
#include "java/com/turbosanta/backend/handle.h"

using ::java_com_turbosanta_backend::sound::SoundPlayer;
using ::java_com_turbosanta_backend::setHandle;

/*
 * Class:     com_turbosanta_backend_sound_NativeAudio
 * Method:    init
 * Signature: ()V
 */
void Java_com_turbosanta_backend_sound_NativeAudio_init(JNIEnv *env, jobject obj) {
  JavaVM* jvm;
  env->GetJavaVM(&jvm);
  SoundPlayer* handle = new SoundPlayer(jvm, env, obj);
  setHandle<SoundPlayer>(env, obj, handle);
}
