#include "back_end/config.h"
#include <curses.h>

#include <algorithm>
#include <vector>

#include <errno.h>
#include <stdio.h>
#include <string.h>

#include "back_end/clocktroller/clocktroller.h"
#include "back_end/graphics/screen.h"

using std::string;
using std::vector;
using back_end::clocktroller::Clocktroller;
using back_end::graphics::Screen;
using back_end::graphics::ScreenRaster;

static const int kNintendoLogoStartPosition = 0x104;

static const vector<unsigned char> kNintendoLogo = {
    0xce, 0xed, 0x66, 0x66, 0xcc, 0x0d, 0x00, 0x0b, 
    0x03, 0x73, 0x00, 0x83, 0x00, 0x0c, 0x00, 0x0d,
    0x00, 0x08, 0x11, 0x1f, 0x88, 0x89, 0x00, 0x0e,
    0xdc, 0xcc, 0x6e, 0xe6, 0xdd, 0xdd, 0xd9, 0x99,
    0xbb, 0xbb, 0x67, 0x63, 0x6e, 0x0e, 0xec, 0xcc,  
    0xdd, 0xdc, 0x99, 0x9f, 0xbb, 0xb9, 0x33, 0x3e
  }; 

class TerminalScreen : public Screen {
 public:
  virtual void Draw(const ScreenRaster& raster) {
    const int realized_width = std::min(COLS, LINES);
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

        move(y * realized_width / ScreenRaster::kScreenHeight, x * 2 * realized_width / ScreenRaster::kScreenWidth);
        addch(real_pixel);
      }
    }
    refresh();
  }
};

vector<unsigned char> BuildROM() {
  const int rom_size = kNintendoLogo.size() + kNintendoLogoStartPosition;
  vector<unsigned char> rom(rom_size, 0x00);
  for (size_t i = 0; i < kNintendoLogo.size(); i++) {
    rom[i + kNintendoLogoStartPosition] = kNintendoLogo[i];
  }
  return rom;
}

vector<unsigned char> ReadROM(string file_name) {
  vector<unsigned char> rom;
  FILE* file = fopen(file_name.c_str(), "r");
  if (file == nullptr) {
    LOG(FATAL) << "Cannot read file: " << strerror(errno);
  }

  const int buffer_size = 1024;
  vector<unsigned char> buffer(buffer_size);
  int amount_read = 0;
  do {
    amount_read = fread(buffer.data(), 1, buffer_size, file);
    // TODO(Brendan): Should not copy the entire buffer since buffer may not be
    // entirely filled.
    rom.insert(rom.end(), buffer.begin(), buffer.end());
  } while (amount_read == buffer_size);

  fclose(file);
  return rom;
}

int main(int argc, char* argv[]) {
  if (argc != 2) {
    printf("Please provide the name of the ROM\n");
    return -1;
  }

  TerminalScreen terminal_screen;
  vector<unsigned char> rom = ReadROM(argv[1]);
  LOG(INFO) << "Finished reading rom";
  Clocktroller clocktroller(&terminal_screen, rom.data(), rom.size());
  LOG(INFO) << "Clocktroller built";

  initscr();
  clocktroller.Setup();
  clocktroller.Start();
  clocktroller.WaitForThreads();
  endwin();
  return 0;
};
