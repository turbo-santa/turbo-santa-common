#include "backend/graphics/graphics_controller.h"

#include <ncurses.h>

namespace back_end {
namespace graphics {

using std::vector;
using memory::BackgroundMap;
using memory::OAMSegment;
using memory::SpriteAttribute;
using memory::Tile;
using memory::TileData;
using memory::VRAMSegment;

namespace {
class TileReflectedX : public Tile {
 public:
  TileReflectedX() : tile_(nullptr) {}

  TileReflectedX(Tile* tile) : tile_(tile) {}

  virtual unsigned char Get(unsigned int y, unsigned int x) {
    return tile_->Get(y, Tile::kTileSize - 1 - x);
  }

  virtual void Set(unsigned int y, unsigned int x, unsigned char value) {
    tile_->Set(y, Tile::kTileSize - 1 - x, value);
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
    return tile_->Get(Tile::kTileSize - 1 - y, x);
  }

  virtual void Set(unsigned int y, unsigned int x, unsigned char value) {
    tile_->Set(Tile::kTileSize - 1 - y, x, value);
  }

  void set_tile(Tile* tile) { tile_ = tile; }

 private:
  Tile* tile_;
};

void RenderTile(Tile* tile, 
                unsigned char y_offset, 
                unsigned char x_offset, 
                MonochromePalette* palette, 
                vector<unsigned char>* screen_buffer_) {
  TileReflectedX x_flip(tile);
  tile = &x_flip;
  for (int y = 0; y < Tile::kTileSize; y++) {
    for (int x = 0; x < Tile::kTileSize; x++) {
      unsigned char color_index = tile->Get(y, x);
      MonochromePalette::Color color = palette->lookup(color_index);
      if (color != MonochromePalette::NONE) {
        unsigned char realized_color = static_cast<unsigned char>(color) * (256 / 4);
        (*screen_buffer_)[(x + x_offset) + (y + y_offset) * kScreenBufferSize] = realized_color;
      }
    }
  }
}

void RenderSprite(SpriteAttribute* sprite_attribute, 
                  GraphicsFlags* graphics_flags, 
                  VRAMSegment* vram_segment, 
                  vector<unsigned char>* screen_buffer_) {
  TileReflectedX tile_reflected_x;
  TileReflectedY tile_reflected_y;
  Tile* tile = vram_segment->lower_tile_data()->tile(sprite_attribute->tile_index());

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
    palette = graphics_flags->object_palette_1();
  } else {
    palette = graphics_flags->object_palette_0();
  }

  RenderTile(tile, sprite_attribute->y(), sprite_attribute->x(), palette, screen_buffer_);
}

void WriteToScreen(GraphicsFlags* graphics_flags, Screen* screen, vector<unsigned char>* screen_buffer_) {
  ScreenRaster* raster = screen->mutable_raster();
  const int y_offset = graphics_flags->scroll_y()->flag();
  const int x_offset = graphics_flags->scroll_x()->flag();
  for (int y = 0; y < ScreenRaster::kScreenHeight; y++) {
    for (int x = 0; x < ScreenRaster::kScreenWidth; x++) {
      raster->Set(y, x, (*screen_buffer_)[(x + x_offset) + (y + y_offset) * kScreenBufferSize]);
    }
  }

  LOG(INFO) << "Rendering screen.";
  screen->Draw();
}

void RenderLowPrioritySprites(GraphicsFlags* graphics_flags, 
                              OAMSegment* oam_segment, 
                              VRAMSegment* vram_segment, 
                              vector<unsigned char>* screen_buffer_) {
  if (!graphics_flags->lcd_control()->sprite_display_enable()) {
    return;
  }
  // LOG(INFO) << "Rendering low priority sprites.";
  for (int i = 0; i < OAMSegment::kAttributeNumber; i++) {
    SpriteAttribute* sprite_attribute = oam_segment->sprite_attribute(i);
    if (!sprite_attribute->over_background()) {
      RenderSprite(sprite_attribute, graphics_flags, vram_segment, screen_buffer_);
    }
  }
}

void RenderHighPrioritySprites(GraphicsFlags* graphics_flags, 
                               OAMSegment* oam_segment, 
                               VRAMSegment* vram_segment, 
                               vector<unsigned char>* screen_buffer_) {
  if (!graphics_flags->lcd_control()->sprite_display_enable()) {
    return;
  }
  // LOG(INFO) << "Rendering high priority sprites.";
  for (int i = 0; i < OAMSegment::kAttributeNumber; i++) {
    SpriteAttribute* sprite_attribute = oam_segment->sprite_attribute(i);
    if (sprite_attribute->over_background()) {
      RenderSprite(sprite_attribute, graphics_flags, vram_segment, screen_buffer_);
    }
  }
}

void RenderBackground(GraphicsFlags* graphics_flags, VRAMSegment* vram_segment, vector<unsigned char>* screen_buffer) {
  LCDControl* lcd_control = graphics_flags->lcd_control();
  if (!lcd_control->bg_display()) {
    // Nothing to do if BG display is unset.
    return;
  }

  BackgroundMap* background;
  if (lcd_control->bg_tile_map_display_select()) {
    background = vram_segment->upper_background_map();
  } else {
    background = vram_segment->lower_background_map();
  }

  TileData* tile_data;
  if (lcd_control->bg_window_tile_data_select()) {
    tile_data = vram_segment->lower_tile_data();
  } else {
    tile_data = vram_segment->upper_tile_data();
  }

  for (int y = 0; y < kScreenBufferSize / Tile::kTileSize; y++) {
    for (int x = 0; x < kScreenBufferSize / Tile::kTileSize; x++) {
      Tile* tile = tile_data->tile(background->Get(y, x));
      RenderTile(tile, y * Tile::kTileSize, x * Tile::kTileSize, graphics_flags->background_palette(), screen_buffer);
    }
  }
}

void RenderWindow(GraphicsFlags* graphics_flags, VRAMSegment* vram_segment, vector<unsigned char>* screen_buffer) {
  LCDControl* lcd_control = graphics_flags->lcd_control();
  int y_offset = graphics_flags->window_y_position()->flag();
  y_offset += graphics_flags->scroll_y()->flag();
  int x_offset = graphics_flags->window_x_position()->flag() + 7;
  x_offset += graphics_flags->scroll_x()->flag();

  if (!lcd_control->window_display_enable()) {
    return; // Nothing to do if disabled.
  }

  BackgroundMap* background;
  if (lcd_control->window_tile_map_display_select()) {
    background = vram_segment->upper_background_map();
  } else {
    background = vram_segment->lower_background_map();
  }

  TileData* tile_data;
  if (lcd_control->bg_window_tile_data_select()) {
    tile_data = vram_segment->upper_tile_data();
  } else {
    tile_data = vram_segment->lower_tile_data();
  }

  for (int y = 0; y < (kScreenBufferSize - y_offset) / Tile::kTileSize; y++) {
    for (int x = 0; x < (kScreenBufferSize - x_offset) / Tile::kTileSize; x++) {
      Tile* tile = tile_data->tile(background->Get(y, x));
      RenderTile(tile,
                 y * Tile::kTileSize + y_offset,
                 x * Tile::kTileSize + x_offset,
                 graphics_flags->background_palette(),
                 screen_buffer);
    }
  }
}

void Clear(vector<unsigned char>* screen_buffer_) {
  for (int y = 0; y < kScreenBufferSize; y++) {
    for (int x = 0; x < kScreenBufferSize; x++) {
      (*screen_buffer_)[x + y * kScreenBufferSize] = 0;
    }
  }
}

void Draw(GraphicsFlags* graphics_flags, OAMSegment* oam_segment, VRAMSegment* vram_segment, Screen* screen) {
  vector<unsigned char> screen_buffer(kScreenBufferSize * kScreenBufferSize, 0);
  LOG(INFO) << "Clearing...";
  Clear(&screen_buffer);
  LOG(INFO) << "Rendering rendering low priority sprites...";
  RenderLowPrioritySprites(graphics_flags, oam_segment, vram_segment, &screen_buffer);
  LOG(INFO) << "Rendering background...";
  RenderBackground(graphics_flags, vram_segment, &screen_buffer);
  LOG(INFO) << "Rendering window...";
  RenderWindow(graphics_flags, vram_segment, &screen_buffer);
  LOG(INFO) << "Rendering rendering high priority sprites...";
  RenderHighPrioritySprites(graphics_flags, oam_segment, vram_segment, &screen_buffer);
  LOG(INFO) << "Rendering...";
  WriteToScreen(graphics_flags, screen, &screen_buffer);
}
} // namespace

void GraphicsController::Init() {
  // TODO(Brendan): Add the flags from graphics_flags_.
  add_memory_segment(&vram_segment_);
  add_memory_segment(&oam_segment_);

  for (auto flag : graphics_flags_.flags()) {
    add_flag(flag);
  }
}

void GraphicsController::Tick(unsigned int number_of_cycles) {
  LCDStatus* lcd_status = graphics_flags_.lcd_status();
  LYCoordinate* ly_coordinate = graphics_flags_.ly_coordinate();
  LYCompare* ly_compare = graphics_flags_.ly_compare();

  if (ly_coordinate->flag() == ly_compare->flag()) {
    lcd_status->set_coincidence_flag(true);
    if (lcd_status->coincidence_interrupt()) {
      interrupt_flag()->set_lcd_stat(true);
    }
  }

  if (ly_coordinate->has_reset()) {
    time_ = 0;
    ly_coordinate->clear_reset();
  }

  time_ += number_of_cycles;
  time_ %= kLargePeriod;
  // Mode 1.
  if (time_ > kVBlankLowerBound) {
    lcd_status->set_mode(LCDStatus::V_BLANK);
    EnableOAM();
    EnableVRAM();
    if (previous_mode_ != MODE_1) {
      SetVBlankInterrupt();
      if (lcd_status->v_blank_interrupt()) {
        SetLCDSTATInterrupt();
      }
    }
    previous_mode_ = MODE_1;
  // Mode 2.
  } else if (time_ % kSmallPeriod < kOAMLockedUpperBound) {
    lcd_status->set_mode(LCDStatus::OAM_LOCKED);
    DisableOAM();
    EnableVRAM();
    if (previous_mode_ != MODE_2) {
      ly_coordinate->Increment();
      if (lcd_status->oam_interrupt()) {
        SetLCDSTATInterrupt();
      }
    }
    previous_mode_ = MODE_2;
  // Mode 3.
  } else if (time_ % kSmallPeriod < kVRAMOAMLockedUpperBound) {
    lcd_status->set_mode(LCDStatus::VRAM_OAM_LOCKED);
    DisableOAM();
    DisableVRAM();
    if (graphics_flags_.lcd_control()->lcd_display_enable() 
        && previous_mode_ != MODE_3
        && ly_coordinate->flag() == 0) {
      Draw(&graphics_flags_, &oam_segment_, &vram_segment_, screen_);
    }
    previous_mode_ = MODE_3;
  // Mode 0.
  } else if (time_ % kSmallPeriod < kHBlankUpperBound) {
    lcd_status->set_mode(LCDStatus::H_BLANK);
    EnableOAM();
    EnableVRAM();
    if (lcd_status->h_blank_interrupt() && previous_mode_ != MODE_0) {
      SetLCDSTATInterrupt();
    }
    previous_mode_ = MODE_0;
  }
}

} // namespace graphics
} // namespace back_end
