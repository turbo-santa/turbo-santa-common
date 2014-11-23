#include "back_end/config.h"
#include <curses.h>

#include <vector>

#include "back_end/clocktroller/clocktroller.h"
#include "back_end/graphics/screen.h"

using back_end::clocktroller::Clocktroller;
using back_end::graphics::Screen;
using back_end::graphics::ScreenRaster;

class TerminalScreen : public Screen {
 public:
  virtual void Draw(const ScreenRaster& raster) {
    for (int y = 0; y < ScreenRaster::kScreenHeight; y++) {
      for (int x = 0; x < ScreenRaster::kScreenWidth; x++) {
        unsigned char pixel_shade = raster.Get(y, x);
        char real_pixel;
        if (pixel_shade <= 64) {
          real_pixel = ' ';
        } else if (pixel_shade <= 128) {
          real_pixel = '.';
        } else if (pixel_shade <= 192) {
          real_pixel = '%';
        } else {
          real_pixel = '#';
        }

        // int actual_y = y * 2 * LINES / ScreenRaster::kScreenHeight - 20;
        // if (actual_y < 0) {
        //   actual_y = 0;
        // }
        // int actual_x = x * 2 * COLS / ScreenRaster::kScreenWidth - 170;
        // if (actual_x < 0) {
        //   actual_x = 0;
        // }

        move(y, x);
        addch(real_pixel);
      }
    }
    refresh();
  }
};

int main() {
  initscr();
  TerminalScreen terminal_screen;
  std::vector<unsigned char> nintendo_logo = {
    0xce, 0xed, 0x66, 0x66, 0xcc, 0x0d, 0x00, 0x0b, 
    0x03, 0x73, 0x00, 0x83, 0x00, 0x0c, 0x00, 0x0d,
    0x00, 0x08, 0x11, 0x1f, 0x88, 0x89, 0x00, 0x0e,
    0xdc, 0xcc, 0x6e, 0xe6, 0xdd, 0xdd, 0xd9, 0x99,
    0xbb, 0xbb, 0x67, 0x63, 0x6e, 0x0e, 0xec, 0xcc,  
    0xdd, 0xdc, 0x99, 0x9f, 0xbb, 0xb9, 0x33, 0x3e
  };
  Clocktroller clocktroller(&terminal_screen, nintendo_logo.data(), nintendo_logo.size());
  clocktroller.Setup();
  clocktroller.Start();
  clocktroller.WaitForThreads();
  endwin();
  return 0;
};
