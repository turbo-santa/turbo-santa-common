#ifndef TURBO_SANTA_COMMON_BACK_END_GRAPHICS_GRAPHICS_CONTROLLER_H_
#define TURBO_SANTA_COMMON_BACK_END_GRAPHICS_GRAPHICS_CONTROLLER_H_

#include <vector>

namespace back_end {
namespace graphics {

class GraphicsController {
 public:
  void Tick(unsigned int number_of_cycles);

  static const int kSmallPeriod = 0;
  static const int kLargePeriod = 1;
  static const int kHBlankLowerBound = 0;
  static const int kVBlankLowerBound = 0;
  static const int kOAMLockedLowerBound = 0;
  static const int kVRAMOAMLockedLowerBound = 0;

  enum Mode {
    H_BLANK = 0,
    V_BLANK = 1,
    OAM_LOCKED = 2,
    VRAM_OAM_LOCKED = 3
  };
 private:
  void draw();
  Mode mode();
  void set_mode(Mode mode);
  unsigned long time_ = 0;
  unsigned char lcd_status_ = 0;
  std::vector<unsigned char> screen_;
};

} // namespace graphics
} // namespace back_end

#endif // TURBO_SANTA_COMMON_BACK_END_GRAPHICS_GRAPHICS_CONTROLLER_H_
