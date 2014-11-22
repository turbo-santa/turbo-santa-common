#ifndef TURBO_SANTA_COMMON_BACK_END_GRAPHICS_SCREEN_H_
#define TURBO_SANTA_COMMON_BACK_END_GRAPHICS_SCREEN_H_

#include <vector>
#include <glog/logging.h>

namespace back_end {
namespace graphics {

class ScreenRaster {
 public:
  ScreenRaster() : data_(kScreenWidth * kScreenHeight, 0x00) {}

  unsigned char Get(unsigned int y, unsigned int x) const {
    Check(y, x);
    return data_[x + kScreenWidth * y];
  }

  void Set(unsigned int y, unsigned int x, unsigned char value) {
    Check(y, x);
    data_[x + kScreenWidth * y] = value;
  }

  static const int kScreenHeight = 144;
  static const int kScreenWidth = 160;
  
 private:
  void Check(unsigned int y, unsigned int x) const {
    if (y >= kScreenHeight || x >= kScreenWidth) {
      LOG(FATAL) << "Attempted access out of bounds: y = " << y << " x = " << x;
    }
  }

  std::vector<unsigned char> data_;
};

class Screen {
 public:
  virtual void Draw(const ScreenRaster& raster) = 0;
};

} // namespace graphics
} // namespace back_end

#endif // TURBO_SANTA_COMMON_BACK_END_GRAPHICS_SCREEN_H_
