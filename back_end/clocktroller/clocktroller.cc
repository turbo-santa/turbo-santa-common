#include <thread>
#include <mutex>
#include <unistd.h>
#include <time.h>
#include "back_end/opcode_executor/opcode_executor.h"
#include "clocktroller.h"
#include <glog/logging.h>

namespace back_end {
namespace clocktroller {

using handlers::OpcodeExecutor;

const double CLOCK_RATE = 8388000; // 8.388 MHz

int MAX_INSTRUCTIONS; // TODO: get rid of this hack

unsigned char* raw_rom;
char should_run = 1;
char start = 0;
std::mutex execution_lock;
std::unique_ptr<OpcodeExecutor> executor;
std::thread handler_thread;
std::thread clock_thread;
unsigned int clock_cycles;

void LaunchHandleLoop(Clocktroller* member) {
    member->HandleLoop();
}

void LaunchClockLoop(Clocktroller* member) {
    member->ClockLoop();
}

Clocktroller::Clocktroller(unsigned char* rom, unsigned long length) {
    executor = std::unique_ptr<OpcodeExecutor>(new OpcodeExecutor::OpcodeExecutor(rom, length));
    raw_rom = rom;
    MAX_INSTRUCTIONS = length;
    LOG(INFO) << "Launching Handle Thread";
    std::thread handler_thread(LaunchHandleLoop, this);
    LOG(INFO) << "Handle Thread Launched";
    LOG(INFO) << "Launching Clock Thread";
    std::thread clock_thread(LaunchClockLoop, this);
    LOG(INFO) << "Clock Thread Launched";
}

void Clocktroller::Start() {
    start = 1;
}

void Clocktroller::Pause() {
    should_run = 0;
    handler_thread.join();
    clock_thread.join();
}

void Clocktroller::Resume() {
    if (!should_run) {
        should_run = 1;
        std::thread handler_thread(LaunchHandleLoop, this);
        std::thread clock_thread(LaunchClockLoop, this);
    }
}

void Clocktroller::Terminate() {
    should_run = 0;
    handler_thread.join();
    clock_thread.join();
}

void Clocktroller::WaitForThreads() {
    handler_thread.join();
    clock_thread.join();
}

void Clocktroller::ClockLoop() {
    clock_t start = clock();
    clock_t elapsed;
    LOG(INFO) << "Clock Loop Spinning Up";
    while (!start) {
        usleep(50000); // wait 50ms
    }
    while(should_run && MAX_INSTRUCTIONS > 0) {
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

void Clocktroller::HandleLoop() {
    clock_t clock_start;
    clock_t clock_stop;
    LOG(INFO) << "Handle Loop Spinning Up";
    while (!start) {
        usleep(50000); // wait 50ms
    }
    while(should_run && MAX_INSTRUCTIONS-- > 0) {
        execution_lock.lock();

        // TODO: handle interrupt
        LOG(INFO) << "Reading Instruction";
        clock_cycles = executor->ReadInstruction();

        execution_lock.unlock();
    }
}

} // namespace clocktroller
} // namespace back_end

