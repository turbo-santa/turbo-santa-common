#include <thread>
#include <chrono>
#include <mutex>
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
std::mutex execution_lock;
std::thread handler_thread;
std::thread clock_thread;
unsigned int clock_cycles;

void LaunchHandleLoop(Clocktroller* member) {
    member->HandleLoop();
}

void LaunchClockLoop(Clocktroller* member) {
    member->ClockLoop();
}

Clocktroller::Clocktroller(unsigned char* rom, long length) {
    LOG(INFO) << "Creating OpcodeExecutor";
    executor = new OpcodeExecutor::OpcodeExecutor();
    raw_rom = rom;
    MAX_INSTRUCTIONS = length;
}

void Clocktroller::Setup() {
    LOG(INFO) << "Launching Handle Thread";
    handler_thread = std::thread(LaunchHandleLoop, this);
    LOG(INFO) << "Handle Thread Launched";
    LOG(INFO) << "Launching Clock Thread";
    clock_thread = std::thread(LaunchClockLoop, this);
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
    std::chrono::milliseconds dur(50);
    while (!start) {
        std::this_thread::sleep_for(dur);
    }
    while(should_run && MAX_INSTRUCTIONS > 0) {
        if (execution_lock.try_lock()) {
            elapsed = clock() - start;

            std::chrono::microseconds wait_time(1000 * 1 / (CLOCK_RATE / clock_cycles) - (elapsed / CLOCKS_PER_SEC));
            if (wait_time < 0) wait_time = 0;

            std::this_thread::sleep_for(wait_time);
            start = clock();
            execution_lock.unlock();
        }

    }
}

void Clocktroller::HandleLoop() {
    clock_t clock_start;
    clock_t clock_stop;
    LOG(INFO) << "Handle Loop Spinning Up";
    std::chrono::milliseconds dur(50);
    while (!start) {
        std::this_thread::sleep_for(dur);
    }
    while(should_run && MAX_INSTRUCTIONS-- > 0) {
        execution_lock.lock();

        // TODO: handle interrupt
        clock_cycles = executor->ReadInstruction();

        execution_lock.unlock();
    }
}

} // namespace clocktroller
} // namespace back_end

