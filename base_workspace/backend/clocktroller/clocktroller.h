#ifndef TURBO_SANTA_COMMON_BACK_END_CLOCKTROLLER_CLOCKTROLLER_H_
#define TURBO_SANTA_COMMON_BACK_END_CLOCKTROLLER_CLOCKTROLLER_H_

#include <atomic>
#include <memory>
#include <thread>
#include "backend/graphics/graphics_controller.h"
#include "backend/graphics/screen.h"
#include "backend/opcode_executor/opcode_executor.h"
#include "backend/memory/default_module.h"
#include "backend/memory/dma_transfer.h"
#include "backend/memory/mbc_module.h"
#include "backend/memory/memory_mapper.h"
#include "backend/memory/primary_flags.h"
#include "backend/memory/unimplemented_module.h"

namespace back_end {
namespace clocktroller {

class Clocktroller {
 public:
  Clocktroller(graphics::Screen* screen) : screen_(screen) {}
  void Init(unsigned char* rom, long length);
  void Run();
  void Pause() { is_paused_ = true; }
  void Kill() { is_dead_ = true; }
  void Wait() { thread_.join(); }

 private:
  memory::PrimaryFlags primary_flags_;
  memory::DefaultModule default_module_;
  memory::MBCModule mbc_;
  memory::DMATransferModule dma_transfer_module_;
  memory::UnimplementedModule unimplemented_module_;
  graphics::Screen* screen_;
  std::unique_ptr<handlers::OpcodeExecutor> opcode_executor_;
  std::unique_ptr<graphics::GraphicsController> graphics_controller_;
  bool is_running_;
  std::atomic<bool> is_paused_;
  std::atomic<bool> is_dead_;
  std::thread thread_;

  void ExecutionLoop();
};

} // namespace clocktroller
} // namespace back_end
#endif // TURBO_SANTA_COMMON_BACK_END_CLOCKTROLLER_CLOCKTROLLER_H_
