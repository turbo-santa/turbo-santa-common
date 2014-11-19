#ifndef TURBO_SANTA_COMMON_BACK_END_CLOCKTROLLER_H_
#define TURBO_SANTA_COMMON_BACK_END_CLOCKTROLLER_H_

namespace back_end {
namespace clocktroller {

class Clocktroller {
    public:
        Clocktroller(unsigned char* rom, unsigned long length);
        void Start();
        void Pause();
        void Terminate();
        void WaitForThreads();
    private:
        void HandleLoop();
        void ClockLoop();
        void Pause();
        void Resume();

    friend void LaunchHandleLoop(Clocktroller*);
    friend void LaunchClockLoop(Clocktroller*);
};
} // namespace clocktroller
} // namespace back_end
#endif // TURBO_SANTA_COMMON_BACK_END_CLOCKTROLLER_H_
