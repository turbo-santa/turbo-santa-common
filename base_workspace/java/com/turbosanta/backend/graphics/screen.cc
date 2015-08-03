#include "java/com/turbosanta/backend/graphics/screen.h"

namespace java_com_turbosanta_backend {
namespace graphics {
namespace {
jmethodID GetGetMethodID(JNIEnv* env, jobject obj) {
  jclass klass = env->GetObjectClass(obj);
  jmethodID mid = env->GetMethodID(klass, "getRGB", "(II)I");
  return mid;
}

jmethodID GetSetMethodID(JNIEnv* env, jobject obj) {
  jclass klass = env->GetObjectClass(obj);
  return env->GetMethodID(klass, "setRGB", "(III)V");
}

jobject GetImageJObject(JNIEnv* env, jobject obj) {
  jclass klass = env->GetObjectClass(obj);
  jfieldID fieldID = env->GetFieldID(klass, "image", "Ljava/awt/image/BufferedImage;");
  return env->GetObjectField(obj, fieldID);
}

jmethodID GetDrawMethodID(JNIEnv* env, jobject obj) {
  jclass klass = env->GetObjectClass(obj);
  return env->GetMethodID(klass, "draw", "()V");
}
} // namespace

ScreenRaster::ScreenRaster(JavaVM* jvm, JNIEnv* env, jobject image) :
    jvm_(jvm),
    image_(env->NewGlobalRef(image)),
    getMID_(GetGetMethodID(env, image_)),
    setMID_(GetSetMethodID(env, image_)) {}

Screen::Screen(JavaVM* jvm, JNIEnv* env, jobject screen) : 
    jvm_(jvm),
    screen_(env->NewWeakGlobalRef(screen)),
    drawMID_(GetDrawMethodID(env, screen)),
    raster_(jvm, env, GetImageJObject(env, screen)) {}

} // namespace graphics
} // namespace java_com_turbosanta_backend
