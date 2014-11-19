#include "back_end/graphics/graphics_controller.h"

namespace back_end {
namespace graphics {

using std::vector;
using memory::OAMSegment;
using memory::SpriteAttribute;
using memory::Tile;
using memory::BackgroundMap;

class TileReflectedX : public Tile {
 public:
  TileReflectedX() : tile_(nullptr) {}

  TileReflectedX(Tile* tile) : tile_(tile) {}

  virtual unsigned char Get(unsigned int y, unsigned int x) {
    return tile_->Get(y, Tile::kTileSize - x);
  }

  virtual void Set(unsigned int y, unsigned int x, unsigned char value) {
    tile_->Set(y, Tile::kTileSize - x, value);
  }

  void set_tile(Tile* tile) { tile_ = tile; }

 private:
  Tile* tile_;
};

class TileReflectedY : public Tile {
 public:
  TileReflectedY() : tile_(nullptr) {}

  TileReflectedY(Tile* tile) : tile_(tile) {}

  virtual unsigned char Get(unsigned int y, unsigned int x) {
    return tile_->Get(Tile::kTileSize - y, x);
  }

  virtual void Set(unsigned int y, unsigned int x, unsigned char value) {
    tile_->Set(Tile::kTileSize - y, x, value);
  }

  void set_tile(Tile* tile) { tile_ = tile; }

 private:
  Tile* tile_;
};

void GraphicsController::Tick(unsigned int number_of_cycles) {
  LCDStatus* lcd_status = graphics_flags()->lcd_status();
  time_ += number_of_cycles;
  time_ %= kLargePeriod;
  // Mode 1.
  if (time_ > kVBlankLowerBound) {
    lcd_status->set_mode(LCDStatus::V_BLANK);
    EnableOAM();
    EnableVRAM();
    SetVBlankInterrupt();
    if (lcd_status->v_blank_interrupt()) {
      SetLCDSTATInterrupt();
    }
  // Mode 0.
  } else if (time_ % kSmallPeriod > kHBlankLowerBound) {
    lcd_status->set_mode(LCDStatus::H_BLANK);
    EnableOAM();
    EnableVRAM();
    if (lcd_status->h_blank_interrupt()) {
      SetLCDSTATInterrupt();
    }
  // Mode 3.
  } else if (time_ % kSmallPeriod > kVRAMOAMLockedLowerBound) {
    lcd_status->set_mode(LCDStatus::VRAM_OAM_LOCKED);
    DisableOAM();
    DisableVRAM();
    Draw();
  // Mode 2.
  } else if (time_ % kSmallPeriod > kOAMLockedLowerBound) {
    lcd_status->set_mode(LCDStatus::OAM_LOCKED);
    DisableOAM();
    EnableVRAM();
    if (lcd_status->oam_interrupt()) {
      SetLCDSTATInterrupt();
    }
  }
}

void GraphicsController::Draw() {
  RenderLowPrioritySprites();
  RenderBackground();
  RenderWindow();
  RenderHighPrioritySprites();
}

void GraphicsController::RenderLowPrioritySprites() {
  for (int i = 0; i < OAMSegment::kAttributeNumber; i++) {
    SpriteAttribute* sprite_attribute = oam_segment()->sprite_attribute(i);
    if (!sprite_attribute->over_background()) {
      RenderSprite(sprite_attribute);
    }
  }
}

void GraphicsController::RenderHighPrioritySprites() {
  for (int i = 0; i < OAMSegment::kAttributeNumber; i++) {
    SpriteAttribute* sprite_attribute = oam_segment()->sprite_attribute(i);
    if (sprite_attribute->over_background()) {
      RenderSprite(sprite_attribute);
    }
  }
}

void GraphicsController::RenderBackground() {
  BackgroundMap* background;
}

void GraphicsController::RenderSprite(SpriteAttribute* sprite_attribute) {
  TileReflectedX tile_reflected_x;
  TileReflectedY tile_reflected_y;
  Tile* tile = vram_segment()->lower_tile_data()->tile(sprite_attribute->tile_index());

  // Reflect the tile as appropriate.
  if (sprite_attribute->x_flip()) {
    tile_reflected_x.set_tile(tile);
    tile = &tile_reflected_x;
  }
  if (sprite_attribute->y_flip()) {
    tile_reflected_y.set_tile(tile);
    tile = &tile_reflected_y;
  }

  // Select color palette.
  MonochromePalette* palette;
  if (sprite_attribute->palette()) {
    palette = graphics_flags()->object_palette_1();
  } else {
    palette = graphics_flags()->object_palette_0();
  }

  RenderTile(tile, sprite_attribute->y(), sprite_attribute->x(), palette);
}

void GraphicsController::RenderTile(Tile* tile, unsigned char y_offset, unsigned char x_offset, MonochromePalette* palette) {
  for (int y = 0; y < Tile::kTileSize; y++) {
    for (int x = 0; x < Tile::kTileSize; x++) {
      MonochromePalette::Color color = palette->lookup(tile->Get(y, x));
      if (color != MonochromePalette::NONE) {
        unsigned char realized_color = static_cast<unsigned char>(color) * (256 / 4);
        screen_buffer_[(x + x_offset) + (y + y_offset) * kScreenBufferSize] = realized_color;
      }
    }
  }
}

} // namespace graphics
} // namespace back_end
