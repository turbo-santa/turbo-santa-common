#include "back_end/graphics/graphics_controller.h"

namespace back_end {
namespace graphics {

void GraphicsController::Tick(unsigned int number_of_cycles) {
  time_ += number_of_cycles;
  time_ %= kLargePeriod;
  // Mode 1.
  if (time_ > kVBlankLowerBound) {
    set_mode(V_BLANK);
    EnableORAM();
    EnableVRAM();
    SetVBlankInterrupt();
    if (v_blank_interrupt()) {
      SetLCDSTATInterrupt();
    }
  // Mode 0.
  } else if (time_ % kSmallPeriod > kHBlankLowerBound) {
    set_mode(H_BLANK);
    EnableORAM();
    EnableVRAM();
    if (h_blank_interrupt()) {
      SetLCDSTATInterrupt();
    }
  // Mode 3.
  } else if (time_ % kSmallPeriod > kVRAMOAMLockedLowerBound) {
    set_mode(VRAM_OAM_LOCKED);
    DisableORAM();
    DisableVRAM();
    draw();
  // Mode 2.
  } else if (time_ % kSmallPeriod > kOAMLockedLowerBound) {
    set_mode(OAM_LOCKED);
    DisableORAM();
    EnableVRAM();
    if (oam_interrupt()) {
      SetLCDSTATInterrupt();
    }
  }
}
} // namespace graphics
} // namespace back_end
