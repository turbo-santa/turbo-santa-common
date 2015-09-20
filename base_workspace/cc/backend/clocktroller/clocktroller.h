#ifndef TURBO_SANTA_COMMON_BACK_END_CLOCKTROLLER_CLOCKTROLLER_H_
#define TURBO_SANTA_COMMON_BACK_END_CLOCKTROLLER_CLOCKTROLLER_H_

#include <atomic>
#include <memory>
#include <thread>

#include "cc/backend/debug/master.h"
#include "cc/backend/memory/memory.h"
#include "cc/backend/memory/memory_mapper.h"
#include "cc/backend/opcode_executor/opcode_executor.h"

namespace backend {
namespace clocktroller {

class Clocktroller {
 public:
  Clocktroller(graphics::Screen* screen) : screen_(screen) {}
  void Init(unsigned char* rom, long length);
  void Run();
  void Pause() { is_paused_ = true; }
  void Kill() { is_dead_ = true; }
  void Wait() { thread_.join(); }
  memory::JoypadFlag* joypad_flag() { return memory_.joypad_flag(); }

 private:
  debug::Master master_;
  memory::Memory memory_;
  graphics::Screen* screen_;
  std::unique_ptr<opcode_executor::OpcodeExecutor> opcode_executor_;
  bool is_running_;
  std::atomic<bool> is_paused_;
  std::atomic<bool> is_dead_;
  std::thread thread_;

  void ExecutionLoop();
};

} // namespace clocktroller
} // namespace backend
#endif // TURBO_SANTA_COMMON_BACK_END_CLOCKTROLLER_CLOCKTROLLER_H_
