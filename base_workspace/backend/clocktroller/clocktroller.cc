#include "backend/clocktroller/clocktroller.h"

#include "submodules/glog/src/glog/logging.h"

namespace back_end {
namespace clocktroller {

using std::unique_ptr;
using graphics::GraphicsController;
using memory::MemoryMapper;
using opcode_executor::OpcodeExecutor;

void Clocktroller::Init(unsigned char* rom, long length) {
  unique_ptr<MemoryMapper> memory_mapper = unique_ptr<MemoryMapper>(new MemoryMapper());

  unimplemented_module_.Init();
  memory_mapper->RegisterModule(unimplemented_module_);

  primary_flags_.Init();
  memory_mapper->RegisterModule(primary_flags_);

  default_module_.Init();
  memory_mapper->RegisterModule(default_module_);

  dma_transfer_module_.Init(memory_mapper.get());
  memory_mapper->RegisterModule(dma_transfer_module_);

  joypad_module_.Init(primary_flags_.interrupt_flag());
  memory_mapper->RegisterModule(joypad_module_);

  mbc_.Init(rom, length);
  memory_mapper->RegisterModule(mbc_);

  graphics_controller_ = unique_ptr<GraphicsController>(new GraphicsController(screen_, &primary_flags_));
  graphics_controller_->Init();
  memory_mapper->RegisterModule(*graphics_controller_);

  opcode_executor_ = unique_ptr<OpcodeExecutor>(
      new OpcodeExecutor(std::move(memory_mapper), 
                         &primary_flags_, 
                         mbc_.internal_rom_flag()
                         ));
  opcode_executor_->Init();
}

void Clocktroller::Run() {
  is_paused_ = false;
  is_dead_ = false;
  if (!is_running_) {
    thread_ = std::thread([this]() { this->ExecutionLoop(); });
  }
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
        LOG(ERROR) << "Clock clock cycles were negative.";
        is_dead_ = true;
      } else {
        graphics_controller_->Tick(ticks);
      }
    }
  }
}

} // namespace clocktroller
} // namespace back_end
