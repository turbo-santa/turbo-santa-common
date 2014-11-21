#include <curses.h>

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
        if (pixel_shade < 64) {
          real_pixel = ' ';
        } else if (pixel_shade < 128) {
          real_pixel = '.';
        } else if (pixel_shade < 192) {
          real_pixel = 'O';
        } else {
          real_pixel = '#';
        }

        move(y, x);
        addch(real_pixel);
      }
    }
  }
};

int main() {
  initscr();
  TerminalScreen terminal_screen;
  Clocktroller clocktroller(&terminal_screen);
  clocktroller.Setup();
  clocktroller.Start();
  clocktroller.WaitForThreads();
  endwin();
  return 0;
};
