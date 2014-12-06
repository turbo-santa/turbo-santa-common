#ifndef TURBO_SANTA_COMMON_BACK_END_TURBO_SANTA_H_
#define TURBO_SANTA_COMMON_BACK_END_TURBO_SANTA_H_

#include <functional>

namespace back_end {
namespace clocktroller {
  class Clocktroller;
}
}
namespace back_end {
namespace graphics {
  class Screen;
}
}

class TurboSanta {
	public:
    TurboSanta();
    ~TurboSanta();
		void init(unsigned char* rom, int length, void(*videoCallback)(const signed char* bitmap, int length));
		void launch();
    void stop();
		void handleInput(unsigned char inputMap);
  private:
    std::unique_ptr<back_end::clocktroller::Clocktroller> clocktroller;
    std::unique_ptr<back_end::graphics::Screen> turbo_screen;
};
#endif
