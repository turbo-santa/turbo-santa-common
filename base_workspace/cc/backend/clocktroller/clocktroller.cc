#include "cc/backend/clocktroller/clocktroller.h"

#include "glog/logging.h"

namespace backend {
namespace clocktroller {

using std::unique_ptr;
using graphics::GraphicsController;
using memory::MemoryMapper;
using opcode_executor::OpcodeExecutor;


void Clocktroller::Init(unsigned char* rom, long length) {
  memory_.Init(rom, length, screen_, sound_player_);
  opcode_executor_ = unique_ptr<OpcodeExecutor>(
      new OpcodeExecutor(memory_.memory_mapper(), 
                         memory_.primary_flags(), 
                         memory_.internal_rom_flag()
                         ));
  opcode_executor_->Init();
}

void Clocktroller::Run() {
  is_paused_ = false;
  is_dead_ = false;
  if (!is_running_) {
    thread_ = std::thread([this]() { this->ExecutionLoop(); });
  }
  LOG(INFO) << "Started ExecutionLoop";
  is_running_ = true;
}

void Clocktroller::ExecutionLoop() {
  for (;;) {
    if (!is_paused_) {
      if (is_dead_) {
        return;
      }
      int ticks = opcode_executor_->ReadInstruction();
      if (ticks < 0) {
        is_dead_ = true;
      } else {
        memory_.Tick(ticks);
      }
    }
  }
}

} // namespace clocktroller
} // namespace backend
