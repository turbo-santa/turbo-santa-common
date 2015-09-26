#include "java/com/turbosanta/backend/sound/com_turbosanta_backend_sound_NativeAudio.h"

static JavaVM* jvm;

/*
 * Class:     com_turbosanta_backend_sound_NativeAudio
 * Method:    init
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_com_turbosanta_backend_sound_NativeAudio_init(JNIEnv *, jobject) {

}

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* jvm_, void*) {
  jvm = jvm_;
  return JNI_VERSION_1_8;
}