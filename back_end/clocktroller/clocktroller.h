#ifndef TURBO_SANTA_COMMON_BACK_END_CLOCKTROLLER_CLOCKTROLLER_H_
#define TURBO_SANTA_COMMON_BACK_END_CLOCKTROLLER_CLOCKTROLLER_H_

#include "back_end/config.h"

#include <atomic>
#include <memory>
#include <mutex>
#include <thread>

#include "back_end/debugger/great_library.h"
#include "back_end/graphics/screen.h"
#include "back_end/graphics/graphics_controller.h"

namespace back_end {
namespace handlers {
class OpcodeExecutor;
} // namespace handlers
} // namespace back_end

namespace back_end {
namespace clocktroller {

class Clocktroller {
    public:
        Clocktroller(graphics::Screen* screen, debugger::GreatLibrary* great_library, unsigned char* rom, long length, bool setup = false);
        Clocktroller(unsigned char* rom, long length);
        void Setup();
        void Start();
        void Pause();
        void Resume();
        void Terminate();
        void WaitForThreads();
        void HandleInput(unsigned char inputMap);
        // TODO(Diego): You need to clean this up when you are done. I do not
        // think there is any good reason not to put this in a unique_ptr.
        handlers::OpcodeExecutor* executor; // TODO: This needs to be private. 
                                            // The only other class that references it is ClocktrollerTest.
    private:
        void HandleLoop();
        void ClockLoop();
        bool should_run = 1;
        std::atomic<bool> start_;
        unsigned char* raw_rom;
        std::mutex execution_lock;
        std::thread handler_thread;
        std::thread clock_thread;
        int clock_cycles;

    friend void LaunchHandleLoop(Clocktroller*);
    friend void LaunchClockLoop(Clocktroller*);
    friend class ClocktrollerTest;
};
} // namespace clocktroller
} // namespace back_end
#endif // TURBO_SANTA_COMMON_BACK_END_CLOCKTROLLER_CLOCKTROLLER_H_
