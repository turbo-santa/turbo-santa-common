#ifndef TURBO_SANTA_COMMON_BACK_END_GRAPHICS_GRAPHICS_CONTROLLER_H_
#define TURBO_SANTA_COMMON_BACK_END_GRAPHICS_GRAPHICS_CONTROLLER_H_

#include <vector>

#include "back_end/graphics/graphics_flags.h"
#include "back_end/graphics/screen.h"
#include "back_end/memory/interrupt_flag.h"
#include "back_end/memory/memory_mapper.h"
#include "back_end/memory/vram_segment.h"

namespace back_end {
namespace graphics {

class MonochromePalette;

class GraphicsController {
 public:
  GraphicsController(memory::MemoryMapper* memory_mapper, Screen* screen) : 
      screen_buffer_(kScreenBufferSize * kScreenBufferSize, 0x00),
      memory_mapper_(memory_mapper),
      screen_(screen) {}

  void Tick(unsigned int number_of_cycles);

  static const int kSmallPeriod = 456;
  static const int kLargePeriod = 70224;
  static const int kVBlankLowerBound = kLargePeriod - kSmallPeriod * 10; // Mode 1.
  static const int kOAMLockedLowerBound = 80; // Mode 2.
  static const int kVRAMOAMLockedLowerBound = 172 + kOAMLockedLowerBound; // Mode 3.
  static const int kHBlankLowerBound = 204 + kVRAMOAMLockedLowerBound; // Mode 0.

  static const int kScreenBufferSize = 256; // Square.

 private:
  GraphicsFlags* graphics_flags() { return memory_mapper_->graphics_flags(); }
  memory::VRAMSegment* vram_segment() { return memory_mapper_->vram_segment(); }
  memory::OAMSegment* oam_segment() { return memory_mapper_->oam_segment(); }
  memory::InterruptFlag* interrupt_flag() { return memory_mapper_->interrupt_flag(); }
  Screen* screen() { return screen_; }

  void Draw();
  void WriteToScreen();
  void SetLCDSTATInterrupt() { interrupt_flag()->set_lcd_stat(true); }
  void SetVBlankInterrupt() { interrupt_flag()->set_v_blank(true); }
  void RenderLowPrioritySprites();
  void RenderHighPrioritySprites();
  void RenderBackground();
  void RenderWindow();
  void RenderSprite(memory::SpriteAttribute* sprite_attribute);
  void RenderTile(memory::Tile* tile, unsigned char y, unsigned char x, MonochromePalette* palette);

  void EnableVRAM() { vram_segment()->Enable(); }
  void EnableOAM() { oam_segment()->Enable(); }
  void DisableVRAM() { vram_segment()->Disable(); }
  void DisableOAM() { oam_segment()->Disable(); }

  unsigned long time_ = 0;
  std::vector<unsigned char> screen_buffer_;
  memory::MemoryMapper* memory_mapper_;
  Screen* screen_;
};

} // namespace graphics
} // namespace back_end

#endif // TURBO_SANTA_COMMON_BACK_END_GRAPHICS_GRAPHICS_CONTROLLER_H_
