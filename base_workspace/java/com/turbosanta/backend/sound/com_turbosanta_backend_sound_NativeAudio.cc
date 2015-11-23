#include "java/com/turbosanta/backend/sound/com_turbosanta_backend_sound_NativeAudio.h"

#include "cc/backend/sound/sound_player.h"
#include "java/com/turbosanta/backend/sound/sound_player.h"
#include "java/com/turbosanta/backend/handle.h"

using ::java_com_turbosanta_backend::sound::SoundPlayer;
using ::java_com_turbosanta_backend::setHandle;

static JavaVM* jvm;

/*
 * Class:     com_turbosanta_backend_sound_NativeAudio
 * Method:    init
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_com_turbosanta_backend_sound_NativeAudio_init(JNIEnv *env, jobject obj) {
  SoundPlayer* handle = new SoundPlayer(jvm, env, obj);
  setHandle<SoundPlayer>(env, obj, handle);
}

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* jvm_, void*) {
  jvm = jvm_;
  return JNI_VERSION_1_8;
}