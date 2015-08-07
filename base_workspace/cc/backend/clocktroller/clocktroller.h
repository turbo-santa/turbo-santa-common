#ifndef TURBO_SANTA_COMMON_BACK_END_CLOCKTROLLER_CLOCKTROLLER_H_
#define TURBO_SANTA_COMMON_BACK_END_CLOCKTROLLER_CLOCKTROLLER_H_

#include <atomic>
#include <memory>
#include <thread>
#include "cc/backend/graphics/graphics_controller.h"
#include "cc/backend/graphics/screen.h"
#include "cc/backend/memory/default_module.h"
#include "cc/backend/memory/dma_transfer.h"
#include "cc/backend/memory/joypad_module.h"
#include "cc/backend/memory/timer/timer_module.h"
#include "cc/backend/memory/mbc_module.h"
#include "cc/backend/memory/memory_mapper.h"
#include "cc/backend/memory/primary_flags.h"
#include "cc/backend/memory/unimplemented_module.h"
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
  memory::JoypadFlag* joypad_flag() { return joypad_module_.joypad_flag(); }

 private:
  memory::PrimaryFlags primary_flags_;
  memory::DefaultModule default_module_;
  memory::JoypadModule joypad_module_;
  memory::timer::TimerModule timer_module_;
  memory::MBCModule mbc_;
  memory::DMATransferModule dma_transfer_module_;
  memory::UnimplementedModule unimplemented_module_;
  graphics::Screen* screen_;
  std::unique_ptr<opcode_executor::OpcodeExecutor> opcode_executor_;
  std::unique_ptr<graphics::GraphicsController> graphics_controller_;
  bool is_running_;
  std::atomic<bool> is_paused_;
  std::atomic<bool> is_dead_;
  std::thread thread_;

  void ExecutionLoop();
};

} // namespace clocktroller
} // namespace backend
#endif // TURBO_SANTA_COMMON_BACK_END_CLOCKTROLLER_CLOCKTROLLER_H_
