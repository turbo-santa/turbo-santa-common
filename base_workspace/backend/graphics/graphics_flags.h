#ifndef TURBO_SANTA_COMMON_BACK_END_GRAPHICS_GRAPHICS_FLAGS_H_
#define TURBO_SANTA_COMMON_BACK_END_GRAPHICS_GRAPHICS_FLAGS_H_

#include "backend/memory/flags.h"
#include "backend/memory/memory_segment.h"
#include "submodules/glog/src/glog/logging.h"

namespace back_end {
namespace graphics {

class LCDControl : public memory::Flag {
 public:
  LCDControl() : memory::Flag(0xff40) {}
  bool lcd_display_enable() { return bit(7); }
  bool window_tile_map_display_select() { return bit(6); }
  bool window_display_enable() { return bit(5); }
  bool bg_window_tile_data_select() { return bit(4); }
  bool bg_tile_map_display_select() { return bit(3); }
  bool sprite_size() { return bit(2); }
  bool sprite_display_enable() { return bit(1); }
  bool bg_display() { return bit(0); }

  void Write(unsigned short address, unsigned char value) override {
    Flag::Write(address, value);
    if (lcd_display_enable()) {
      LOG(INFO) << "LCD enabled";
    } else {
      LOG(INFO) << "LCD disabled";
    }
  }
};

class LCDStatus : public memory::Flag {
 public:
  enum Mode {
    H_BLANK = 0,
    V_BLANK = 1,
    OAM_LOCKED = 2,
    VRAM_OAM_LOCKED = 3
  };

  LCDStatus() : memory::Flag(0xff41) {}

  Mode mode() { return static_cast<Mode>(0b00000011 & flag()); }
  bool coincidence_interrupt() { return bit(6); }
  bool oam_interrupt() { return bit(5); }
  bool v_blank_interrupt() { return bit(4); }
  bool h_blank_interrupt() { return bit(3); }
  bool coincidence_flag() { return bit(2); }

  void set_mode(Mode mode) {
    set_flag(flag() & 0b11111100);
    set_flag(flag() + static_cast<unsigned char>(mode));
  }

  void set_coincidence_flag(bool value) { set_bit(2, value); }

};

class ScrollY : public memory::Flag {
 public:
  ScrollY() : memory::Flag(0xff42) {}
};

class ScrollX : public memory::Flag {
 public:
  ScrollX() : memory::Flag(0xff43) {}
};

// Specifies the Y coordinate currently being rendered to the screen.
class LYCoordinate : public memory::Flag {
 public:
  LYCoordinate() : memory::Flag(0xff44) {}
  // LY is a READ ONLY register.
  virtual void Write(unsigned short, unsigned char) {
    set_flag(0);
    has_reset_ = 0;
  }

  virtual void set_flag(unsigned char value) { Flag::set_flag(value); }

  bool has_reset() { return has_reset_; }

  void clear_reset() { has_reset_ = false; }

  void Increment() {
    LOG(INFO) << "LY Coordinate: Increment called, current value = 0x" << std::hex << (0x0000 + flag());
    if (flag() >= 153) {
      set_flag(0);
    } else {
      set_flag(flag() + 1);
    }
  }

 private:
  bool has_reset_ = false;
};

// Specifies the Y coordinate to for which to set the cooincident bit in LCD STAT.
class LYCompare : public memory::Flag {
 public:
  LYCompare() : memory::Flag(0xff45) {}
};

class WindowYPosition : public memory::Flag {
 public:
  WindowYPosition() : memory::Flag(0xff4a) {}
};

class WindowXPosition : public memory::Flag {
 public:
  WindowXPosition() : memory::Flag(0xff4b) {}
};

class MonochromePalette : public memory::Flag {
 public:
  enum Color {
    WHITE = 0,
    LIGHT_GRAY = 1,
    DARK_GRAY = 2,
    BLACK = 3,
    NONE = 4
  };

  MonochromePalette(unsigned short address) : memory::Flag(address) {}

  virtual Color lookup(unsigned char index) { return static_cast<Color>((flag() >> (index * 2)) & 0b00000011); }
};

class ObjectPalette : public MonochromePalette {
 public:
  ObjectPalette(unsigned short address) : MonochromePalette(address) {}

  virtual Color lookup(unsigned char index) {
    if (index == 0) {
      return Color::NONE;
    } else {
      return MonochromePalette::lookup(index);
    }
  }
};

class BackgroundPalette : public MonochromePalette {
 public:
  BackgroundPalette() : MonochromePalette(0xff47) {}
};

class ObjectPalette0 : public ObjectPalette {
 public:
  ObjectPalette0() : ObjectPalette(0xff48) {}
};

class ObjectPalette1 : public ObjectPalette {
 public:
  ObjectPalette1() : ObjectPalette(0xff49) {}
};

// TODO(Brendan): Move this into a .cc.
// TODO(Brendan): Use a more general way to manipulate these flags, could use a
// map.
class GraphicsFlags : public memory::MemorySegment {
 public:
  GraphicsFlags() {
    flags_.push_back(&lcd_control_);
    flags_.push_back(&lcd_status_);
    flags_.push_back(&scroll_y_);
    flags_.push_back(&scroll_x_);
    flags_.push_back(&ly_coordinate_);
    flags_.push_back(&ly_compare_);
    flags_.push_back(&window_y_position_);
    flags_.push_back(&window_x_position_);
    flags_.push_back(&background_palette_);
    flags_.push_back(&object_palette_0_);
    flags_.push_back(&object_palette_1_);
  }

  virtual bool InRange(unsigned short address) {
    return lcd_control_.InRange(address) ||
        lcd_status_.InRange(address) ||
        scroll_y_.InRange(address) ||
        scroll_x_.InRange(address) ||
        ly_coordinate_.InRange(address) ||
        ly_compare_.InRange(address) ||
        window_y_position_.InRange(address) ||
        window_x_position_.InRange(address) ||
        background_palette_.InRange(address) ||
        object_palette_0_.InRange(address) ||
        object_palette_1_.InRange(address);
  }

  virtual unsigned char Read(unsigned short address) {
    if (lcd_control_.InRange(address)) {
      return lcd_control_.Read(address);
    } else if (lcd_status_.InRange(address)) {
      LOG(INFO) << "Checked LCD Status.";
      return lcd_status_.Read(address);
    } else if (scroll_y_.InRange(address)) {
      return scroll_y_.Read(address);
    } else if (scroll_x_.InRange(address)) {
      return scroll_x_.Read(address);
    } else if (ly_coordinate_.InRange(address)) {
      LOG(INFO) << "Checked LY LYCoordinate";
      return ly_coordinate_.Read(address);
    } else if (ly_compare_.InRange(address)) {
      return ly_compare_.Read(address);
    } else if (window_y_position_.InRange(address)) {
      return window_y_position_.Read(address);
    } else if (window_x_position_.InRange(address)) {
      return window_x_position_.Read(address);
    } else if (background_palette_.InRange(address)) {
      return background_palette_.Read(address);
    } else if (object_palette_0_.InRange(address)) {
      return object_palette_0_.Read(address);
    } else if (object_palette_1_.InRange(address)) {
      return object_palette_1_.Read(address);
    } else {
      LOG(FATAL) << "Address outside of range: " << address;
    }
  }

  virtual void Write(unsigned short address, unsigned char value) {
    if (lcd_control_.InRange(address)) {
      lcd_control_.Write(address, value);
      LOG(INFO) << "LCD control written to.";
      LOG(INFO) << "lcd_control = " << lcd_control_.lcd_display_enable();
      LOG(INFO) << "window_tile_map_display_select = " << lcd_control_.window_tile_map_display_select();
      LOG(INFO) << "window_display_enable = " << lcd_control_.window_display_enable();
      LOG(INFO) << "bg_window_tile_data_select = " << lcd_control_.bg_window_tile_data_select();
      LOG(INFO) << "bg_tile_map_display_select = " << lcd_control_.bg_tile_map_display_select();
      LOG(INFO) << "sprite_size = " << lcd_control_.sprite_size();
      LOG(INFO) << "sprite_display_enable = " << lcd_control_.sprite_display_enable();
      LOG(INFO) << "bg_display = " << lcd_control_.bg_display();
    } else if (lcd_status_.InRange(address)) {
      lcd_status_.Write(address, value);
      LOG(INFO) << "LCD status written to.";
      LOG(INFO) << "coincidence_interrupt = " << lcd_status_.coincidence_interrupt();
      LOG(INFO) << "oam_interrupt = " << lcd_status_.oam_interrupt();
      LOG(INFO) << "v_blank_interrupt = " << lcd_status_.v_blank_interrupt();
      LOG(INFO) << "h_blank_interrupt = " << lcd_status_.h_blank_interrupt();
    } else if (scroll_y_.InRange(address)) {
      scroll_y_.Write(address, value);
      LOG(INFO) << "Scroll Y written to.";
    } else if (scroll_x_.InRange(address)) {
      scroll_x_.Write(address, value);
      LOG(INFO) << "Scroll X written to.";
    } else if (ly_coordinate_.InRange(address)) {
      ly_coordinate_.Write(address, value);
    } else if (ly_compare_.InRange(address)) {
      ly_compare_.Write(address, value);
      LOG(INFO) << "LY Compare written to, value = " << value;
    } else if (window_y_position_.InRange(address)) {
      window_y_position_.Write(address, value);
      LOG(INFO) << "Window Y Position set.";
    } else if (window_x_position_.InRange(address)) {
      LOG(INFO) << "Window X Position set.";
      window_x_position_.Write(address, value);
    } else if (background_palette_.InRange(address)) {
      background_palette_.Write(address, value);
      LOG(INFO) << std::hex << 0x0000 + value << " was written to the background palette";
    } else if (object_palette_0_.InRange(address)) {
      object_palette_0_.Write(address, value);
      LOG(INFO) << std::hex << 0x0000 + value << " was written to the object palette 0 palette";
    } else if (object_palette_1_.InRange(address)) {
      object_palette_1_.Write(address, value);
      LOG(INFO) << std::hex << 0x0000 + value << " was written to the object palette 1 palette";
    } else {
      LOG(FATAL) << "Address outside of range: " << address;
    }
  }

  const std::vector<memory::Flag*>& flags() const { return flags_; }

  LCDControl* lcd_control() { return &lcd_control_; }
  LCDStatus* lcd_status() { return &lcd_status_; }
  ScrollX* scroll_x() { return &scroll_x_; }
  ScrollY* scroll_y() { return &scroll_y_; }
  LYCoordinate* ly_coordinate() { return &ly_coordinate_; }
  LYCompare* ly_compare() { return &ly_compare_; }
  WindowYPosition* window_y_position() { return &window_y_position_; }
  WindowXPosition* window_x_position() { return &window_x_position_; }
  MonochromePalette* background_palette() { return &background_palette_; }
  MonochromePalette* object_palette_0() { return &object_palette_0_; }
  MonochromePalette* object_palette_1() { return &object_palette_1_; }
 private:
  std::vector<memory::Flag*> flags_;
  LCDControl lcd_control_;
  LCDStatus lcd_status_;
  ScrollY scroll_y_;
  ScrollX scroll_x_;
  LYCoordinate ly_coordinate_;
  LYCompare ly_compare_;
  WindowYPosition window_y_position_;
  WindowXPosition window_x_position_;
  BackgroundPalette background_palette_;
  ObjectPalette0 object_palette_0_;
  ObjectPalette1 object_palette_1_;
};

} // namespace graphics
} // namespace back_end

#endif // TURBO_SANTA_COMMON_BACK_END_GRAPHICS_GRAPHICS_FLAGS_H_
