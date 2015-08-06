#ifndef TURBO_SANTA_COMMON_BACK_END_GRAPHICS_SCREEN_H_
#define TURBO_SANTA_COMMON_BACK_END_GRAPHICS_SCREEN_H_

#include <vector>
#include "submodules/glog/src/glog/logging.h"

namespace back_end {
namespace graphics {

class ScreenRaster {
 public:
  static const int kScreenHeight = 144;
  static const int kScreenWidth = 160;
  
  virtual uint8_t Get(uint32_t y, uint32_t x) const = 0;

  virtual void Set(uint32_t y, uint32_t x, uint8_t value) = 0;

 protected:
  void Check(unsigned int y, unsigned int x) const {
    if (y >= kScreenHeight || x >= kScreenWidth) {
      LOG(FATAL) << "Attempted access out of bounds: y = " << y << " x = " << x;
    }
  }
};

class DefaultRaster : public ScreenRaster {
 public:
  DefaultRaster() : data_(kScreenWidth * kScreenHeight, 0x00) {}

  uint8_t Get(uint32_t y, uint32_t x) const override {
    Check(y, x);
    return data_[x + kScreenWidth * y];
  }

  virtual void Set(uint32_t y, uint32_t x, uint8_t value) {
    Check(y, x);
    data_[x + kScreenWidth * y] = value;
  }

  static const int kScreenHeight = 144;
  static const int kScreenWidth = 160;
  
 private:
  std::vector<unsigned char> data_;
};

class Screen {
 public:
  virtual void Draw() = 0;

  virtual const ScreenRaster& raster() = 0;

  virtual ScreenRaster* mutable_raster() = 0;
};

} // namespace graphics
} // namespace back_end

#endif // TURBO_SANTA_COMMON_BACK_END_GRAPHICS_SCREEN_H_
