#include "backend/config.h"

#include <chrono>
#include <mutex>
#include <time.h>
#include "backend/opcode_executor/opcode_executor.h"
#include "backend/clocktroller/clocktroller.h"
#include <glog/logging.h>

#include <iostream>
#include <fstream>

namespace back_end {
namespace clocktroller {
using debugger::GreatLibrary;
using graphics::Screen;
using handlers::OpcodeExecutor;
using std::chrono::microseconds;

static const double kClockRate = 8388000; // 8.388 MHz
static const microseconds kZero(0);

int MAX_INSTRUCTIONS; // TODO(Diego): get rid of this hack

void LaunchHandleLoop(Clocktroller* member) {
  member->HandleLoop();
}

void LaunchClockLoop(Clocktroller* member) {
  member->ClockLoop();
}

Clocktroller::Clocktroller(Screen* screen, GreatLibrary* great_library, unsigned char* rom, long length, bool setup) : 
    executor(new OpcodeExecutor(screen, great_library, rom, length)), start_(false) {
  raw_rom = nullptr;
  MAX_INSTRUCTIONS = 2500000;
  if (setup) {
    Setup();
  }
}

Clocktroller::Clocktroller(unsigned char* rom, long length) : start_(false) {
  LOG(INFO) << "Creating OpcodeExecutor";
  executor = new OpcodeExecutor();
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
  start_ = true;
}

void Clocktroller::Pause() {
  should_run = 0;
  handler_thread.join();
  clock_thread.join();
}

void Clocktroller::Resume() {
  if (!should_run) {
    should_run = true;
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

void Clocktroller::HandleInput(unsigned char inputMap) {
  executor->HandleInput(inputMap);
}

void Clocktroller::ClockLoop() {
  clock_t start = clock();
  clock_t elapsed;
  LOG(INFO) << "Clock Loop Spinning Up";
  std::chrono::milliseconds dur(50);
  while (!start_) {
    std::this_thread::sleep_for(dur);
  }
  while(should_run && MAX_INSTRUCTIONS > 0) {
    //        if (execution_lock.try_lock()) {
    elapsed = clock() - start;
    
    std::chrono::microseconds wait_time(static_cast<long>(1000 * 1 / (kClockRate / clock_cycles) - (elapsed / CLOCKS_PER_SEC)));
    if (wait_time < kZero) {
      wait_time = kZero;
    }
    
    // std::this_thread::sleep_for(wait_time);
    start = clock();
    //             execution_lock.unlock();
    //        }
    
  }
}

void Clocktroller::HandleLoop() {
    LOG(INFO) << "Handle Loop Spinning Up";
    std::chrono::milliseconds dur(50);
    while (!start_) {
        std::this_thread::sleep_for(dur);
    }
    while(should_run && MAX_INSTRUCTIONS-- > 0) {
//         execution_lock.lock();

        clock_cycles = executor->ReadInstruction();

        if (clock_cycles == -1) {
          should_run = false;
        }

//         execution_lock.unlock();
    }
}
  
} // namespace clocktroller
} // namespace back_end
