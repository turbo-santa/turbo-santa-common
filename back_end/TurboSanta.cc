#include "TurboSanta.h"
#include "back_end/clocktroller/clocktroller.h"
#include "back_end/graphics/screen.h"

using back_end::clocktroller::Clocktroller;
using back_end::graphics::Screen;
using back_end::graphics::ScreenRaster;
using std::unique_ptr;

class TurboScreen : public Screen {
  public:
    TurboScreen(void(*videoCallback)(const signed char* bitmap, int length)) : videoCallback_(videoCallback), bitmap(new  signed char[ScreenRaster::kScreenHeight * ScreenRaster::kScreenWidth]) {}
    virtual void Draw(const ScreenRaster& raster) {
      for (int y = 0; y < ScreenRaster::kScreenHeight; y++) {
        for (int x = 0; x < ScreenRaster::kScreenWidth; x++) {
          bitmap.get()[x + y * ScreenRaster::kScreenWidth] = (signed)raster.Get(y, x);
        }
      }
      videoCallback_(bitmap.get(), ScreenRaster::kScreenWidth * ScreenRaster::kScreenHeight);
    }
  private:
    void(*videoCallback_)(const signed char* bitmap, int length);
    unique_ptr<signed char> bitmap;
};

TurboSanta::TurboSanta() {}
TurboSanta::~TurboSanta() {}

void TurboSanta::init(unsigned char* rom, int length, void(*videoCallback)(const signed char* bitmap, int length)) {
  turbo_screen = unique_ptr<Screen>(new TurboScreen(videoCallback));
  clocktroller = unique_ptr<Clocktroller>(new Clocktroller(turbo_screen.get(), rom, length, true));
}

void TurboSanta::handleInput(unsigned char) {}

void TurboSanta::launch() {
  clocktroller->Start();
}

void TurboSanta::stop() {
  if (clocktroller != nullptr) {
    clocktroller->Terminate();
  }
}


