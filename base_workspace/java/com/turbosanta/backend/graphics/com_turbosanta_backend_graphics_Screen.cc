#include "java/com/turbosanta/backend/graphics/com_turbosanta_backend_graphics_Screen.h"

#include "backend/graphics/screen.h"
#include "java/com/turbosanta/backend/graphics/screen.h"
#include "java/com/turbosanta/backend/handle.h"

using ::java_com_turbosanta_backend::graphics::Screen;
using ::java_com_turbosanta_backend::getHandleField;
using ::java_com_turbosanta_backend::setHandle;

static JavaVM* jvm;

void Java_com_turbosanta_backend_graphics_Screen_init(JNIEnv* env, jobject obj) {
  Screen* handle = new Screen(jvm, env, obj);
  setHandle<Screen>(env, obj, handle);
}

jint Java_com_turbosanta_backend_graphics_Screen_getWidth(JNIEnv*, jclass) {
  return back_end::graphics::ScreenRaster::kScreenWidth;
}

jint Java_com_turbosanta_backend_graphics_Screen_getHeight(JNIEnv*, jclass) {
  return back_end::graphics::ScreenRaster::kScreenHeight;
}

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* jvm_, void*) {
  jvm = jvm_;
  return JNI_VERSION_1_6;
}
