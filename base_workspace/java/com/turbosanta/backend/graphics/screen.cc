#include "java/com/turbosanta/backend/graphics/screen.h"

namespace java_com_turbosanta_backend {
namespace graphics {
namespace {
jmethodID GetGetMethodID(JNIEnv* env, jobject obj) {
  jclass klass = env->GetObjectClass(obj);
  jmethodID mid = env->GetMethodID(klass, "getRGB", "(II)I");
  std::cout << "env = " << env << " image = " << obj << " getMID = " << mid << std::endl;
  return mid;
}

jmethodID GetSetMethodID(JNIEnv* env, jobject obj) {
  jclass klass = env->GetObjectClass(obj);
  return env->GetMethodID(klass, "setRGB", "(III)V");
}

jobject GetImageJObject(JNIEnv* env, jobject obj) {
  jclass klass = env->GetObjectClass(obj);
  jfieldID fieldID = env->GetFieldID(klass, "image", "Ljava/awt/image/BufferedImage;");
  jobject image = env->GetObjectField(obj, fieldID);
  return image;
}

jmethodID GetDrawMethodID(JNIEnv* env, jobject obj) {
  jclass klass = env->GetObjectClass(obj);
  return env->GetMethodID(klass, "draw", "()V");
}
} // namespace

ScreenRaster::ScreenRaster(JNIEnv* env, jobject image) : 
    env_(env), 
    image_(image),
    getMID_(GetGetMethodID(env, image)),
    setMID_(GetSetMethodID(env, image)) {}

Screen::Screen(JNIEnv* env, jobject screen) : 
    env_(env),
    screen_(screen),
    drawMID_(GetDrawMethodID(env, screen)),
    raster_(env, GetImageJObject(env, screen)) {}

} // namespace graphics
} // namespace java_com_turbosanta_backend
