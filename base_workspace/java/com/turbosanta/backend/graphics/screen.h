#ifndef JAVA_COM_TURBOSANTA_BACKEND_GRAPHICS_SCREEN_H_
#define JAVA_COM_TURBOSANTA_BACKEND_GRAPHICS_SCREEN_H_

#include <cstdint>
#include <jni.h>
#include "backend/graphics/screen.h"

#include <iostream>

namespace java_com_turbosanta_backend {
namespace graphics {

class ScreenRaster : public back_end::graphics::ScreenRaster {
 public:
  ScreenRaster(JNIEnv* env, jobject image);

  uint8_t Get(uint32_t y, uint32_t x) const override {
    jvalue args[2];
    args[0].i = static_cast<jint>(x);
    args[1].i = static_cast<jint>(y);
    jint value = env_->CallIntMethodA(image_, getMID_, reinterpret_cast<const jvalue*>(&args));
    return static_cast<uint8_t>(value);
  }

  void Set(uint32_t y, uint32_t x, uint8_t value) override {
    std::cout << "env = " << env_ << " image = " << image_ << " getMID = " << getMID_ << std::endl;
    jvalue args[3];
    args[0].i = static_cast<jint>(x);
    args[1].i = static_cast<jint>(y);
    args[2].i = static_cast<jint>(value);
    env_->CallVoidMethodA(image_, setMID_, reinterpret_cast<const jvalue*>(&args));
    // env_->CallVoidMethod(image_, setMID_, static_cast<jint>(x), static_cast<jint>(y), static_cast<jint>(value));
  }

 private:
  JNIEnv* env_;
  jobject image_;
  jmethodID getMID_;
  jmethodID setMID_;
};

class Screen : public back_end::graphics::Screen {
 public:
  Screen(JNIEnv* env, jobject screen);

  void Draw() override {
    env_->CallVoidMethod(screen_, drawMID_);
  }

  back_end::graphics::ScreenRaster* mutable_raster() { return &raster_; }

  const back_end::graphics::ScreenRaster& raster() { return raster_; }

 private:
  JNIEnv* env_;
  jobject screen_;
  jmethodID drawMID_;
  ScreenRaster raster_;
};

} // namespace graphics
} // namespace java_com_turbosanta_backend

#endif // JAVA_COM_TURBOSANTA_BACKEND_GRAPHICS_SCREEN_H_
