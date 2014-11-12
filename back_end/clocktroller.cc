#include <thread.h>
#include <mutex.h>
#include <unistd.h>
#include <time.h>
#include "back_end/opcode_executor/opcode_executor.h"


namespace back_end {

const unsigned double CLOCK_RATE = 8388000; // 8.388 MHz

unsigned char* raw_rom;
char should_run = 1;
std::mutex execution_lock;
OpcodeExecutor executor;
unsigned int clock_cycles;

Clocktroller::Clocktroller(unsigned char* rom) : raw_rom(rom), executor(rom) {
    // do any more initialization here
}

Clocktroller::Launch() {
    std::thread handler_thread(HandleLoop);
    std::thread clock_thread(ClockLoop);
}

Clocktroller::Pause() {
    should_run = 0;
    handler_thread.join();
    clock_thread.join();
}

Clocktroller::Resume() {
    if (!should_run) {
        should_run = 1;
        std::thread handler_thread(HandleLoop);
        std::thread clock_thread(ClockLoop);
    }
}

Clocktroller::ClockLoop() {
    clock_t start = clock();
    clock_t elapsed;
    while(should_run) {
        if (execution_lock.try_lock()) {
            elapsed = clock() - start;

            int wait_time = 1 / (CLOCK_RATE / clock_cycles) - (elapsed / CLOCKS_PER_SEC);
            if (wait_time < 0) wait_time = 0;

            usleep(wait_time);
            start = clock();
            execution_lock.unlock();
        }

    }
}

Clocktroller::HandleLoop() {
    clock_t clock_start;
    clock_t clock_stop;
    while(should_run) {
        execution_lock.lock();

        // TODO: handle interrupt
        clock_cycles = executor.ReadInstruction();

        execution_lock.unlock();
    }
}

} // namespace back_end

