#ifndef TURBO_SANTA_COMMON_BACK_END_GRAPHICS_GRAPHICS_CONTROLLER_H_
#define TURBO_SANTA_COMMON_BACK_END_GRAPHICS_GRAPHICS_CONTROLLER_H_

#include <vector>

namespace back_end {
namespace graphics {

class GraphicsController {
 public:
  void Tick(unsigned int number_of_cycles);

  static const int kSmallPeriod = 456;
  static const int kLargePeriod = 70224;
  static const int kVBlankLowerBound = kLargePeriod - kSmallPeriod * 10; // Mode 1.
  static const int kOAMLockedLowerBound = 80; // Mode 2.
  static const int kVRAMOAMLockedLowerBound = 172 + kOAMLockedLowerBound; // Mode 3.
  static const int kHBlankLowerBound = 204 + kVRAMOAMLockedLowerBound; // Mode 0.

  enum Mode {
    H_BLANK = 0,
    V_BLANK = 1,
    OAM_LOCKED = 2,
    VRAM_OAM_LOCKED = 3
  };
 private:
  void draw();
  Mode mode();
  bool coincidence_interrupt();
  bool oam_interrupt();
  bool v_blank_interrupt();
  bool h_blank_interrupt();
  bool coincidence_flag();
  void set_mode(Mode mode);
  void set_coincidence_flag(bool value);
  void SetLCDSTATInterrupt();
  void SetVBlankInterrupt();
  void EnableVRAM();
  void EnableORAM();
  void DisableVRAM();
  void DisableORAM();

  unsigned long time_ = 0;
  unsigned char lcd_status_ = 0;
  std::vector<unsigned char> screen_;
};

} // namespace graphics
} // namespace back_end

#endif // TURBO_SANTA_COMMON_BACK_END_GRAPHICS_GRAPHICS_CONTROLLER_H_
