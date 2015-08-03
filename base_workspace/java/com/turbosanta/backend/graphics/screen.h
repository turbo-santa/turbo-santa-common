#ifndef JAVA_COM_TURBOSANTA_BACKEND_GRAPHICS_SCREEN_H_
#define JAVA_COM_TURBOSANTA_BACKEND_GRAPHICS_SCREEN_H_

#include <cstdint>
#include <jni.h>
#include "backend/graphics/screen.h"

namespace java_com_turbosanta_backend {
namespace graphics {

class ScreenRaster : public back_end::graphics::ScreenRaster {
 public:
  ScreenRaster(JavaVM* jvm, JNIEnv* env, jobject image);

  uint8_t Get(uint32_t y, uint32_t x) const override {
    JNIEnv* env;
    jvm_->AttachCurrentThread((void**) &env, nullptr);
    jvalue args[2];
    args[0].i = static_cast<jint>(x);
    args[1].i = static_cast<jint>(y);
    jint value = env->CallIntMethodA(image_, getMID_, reinterpret_cast<const jvalue*>(&args));
    return static_cast<uint8_t>(value >> 8 & 0xff);
  }

  void Set(uint32_t y, uint32_t x, uint8_t raw_value) override {
    uint32_t value = 0xff - raw_value;
    value = (value << 24) | (value << 16) | (value << 8) | 0xff;

    JNIEnv* env;
    jvm_->AttachCurrentThread((void**) &env, nullptr);
    jvalue args[3];
    args[0].i = static_cast<jint>(x);
    args[1].i = static_cast<jint>(y);
    args[2].i = static_cast<jint>(value);
    env->CallVoidMethodA(image_, setMID_, reinterpret_cast<const jvalue*>(&args));
  }

 private:
  JavaVM* jvm_;
  jobject image_;
  jmethodID getMID_;
  jmethodID setMID_;
};

class Screen : public back_end::graphics::Screen {
 public:
  Screen(JavaVM* jvm, JNIEnv* env, jobject screen);

  void Draw() override {
    JNIEnv* env;
    jvm_->AttachCurrentThread((void**) &env, nullptr);
    env->CallVoidMethod(screen_, drawMID_);
  }

  back_end::graphics::ScreenRaster* mutable_raster() { return &raster_; }

  const back_end::graphics::ScreenRaster& raster() { return raster_; }

 private:
  JavaVM* jvm_;
  jobject screen_;
  jmethodID drawMID_;
  ScreenRaster raster_;
};

} // namespace graphics
} // namespace java_com_turbosanta_backend

#endif // JAVA_COM_TURBOSANTA_BACKEND_GRAPHICS_SCREEN_H_
