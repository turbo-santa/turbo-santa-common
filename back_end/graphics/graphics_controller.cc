#include "back_end/graphics/graphics_controller.h"

namespace back_end {
namespace graphics {
void GraphicsController::Tick(unsigned int number_of_cycles) {
  time_ += number_of_cycles;
  time_ %= kLargePeriod;
  if (time_ >= kVBlankLowerBound) {
    set_mode(V_BLANK);
    // set v-blank interrupt register.
    if (
  } else if (time_ % kSmallPeriod >= kHBlankLowerBound) {
    set_mode(H_BLANK);
  } else if (time_ % kSmallPeriod >= kVRAMOAMLockedLowerBound) {
    set_mode(VRAM_OAM_LOCKED);
    draw();
  } else if (time_ % kSmallPeriod >= kOAMLockedLowerBound) {
    set_mode(OAM_LOCKED);
  }
}
} // namespace graphics
} // namespace back_end
