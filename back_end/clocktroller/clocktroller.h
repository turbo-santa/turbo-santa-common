#ifndef TURBO_SANTA_COMMON_BACK_END_CLOCKTROLLER_CLOCKTROLLER_H_
#define TURBO_SANTA_COMMON_BACK_END_CLOCKTROLLER_CLOCKTROLLER_H_

#include <atomic>

namespace back_end {
namespace handlers {
class OpcodeExecutor;
} // namespace handlers
} // namespace back_end

namespace back_end {
namespace clocktroller {

class Clocktroller {
    public:
        Clocktroller(unsigned char* rom, long length);
        void Setup();
        void Start();
        void Pause();
        void Resume();
        void Terminate();
        void WaitForThreads();
        handlers::OpcodeExecutor* executor; // TODO: This needs to be private. 
                                                            // The only other class that references it is ClocktrollerTest.
    private:
        void HandleLoop();
        void ClockLoop();
        bool should_run = 1;
        std::atomic<bool> start_;
        

    friend void LaunchHandleLoop(Clocktroller*);
    friend void LaunchClockLoop(Clocktroller*);
    friend class ClocktrollerTest;
};
} // namespace clocktroller
} // namespace back_end
#endif // TURBO_SANTA_COMMON_BACK_END_CLOCKTROLLER_CLOCKTROLLER_H_
