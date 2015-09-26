#include "cc/backend/memory/memory.h"

#include <memory>
#include "cc/backend/graphics/graphics_controller.h"
#include "cc/backend/graphics/screen.h"
#include "cc/backend/memory/dma_transfer/dma_transfer.h"
#include "cc/backend/memory/interrupt/primary_flags.h"
#include "cc/backend/memory/joypad/joypad_module.h"
#include "cc/backend/memory/mbc/mbc_module.h"
#include "cc/backend/memory/memory_mapper.h"
#include "cc/backend/memory/ram/default_module.h"
#include "cc/backend/memory/timer/timer_module.h"
#include "cc/backend/memory/unimplemented/unimplemented_module.h"
#include "cc/backend/opcode_executor/opcode_executor.h"
#include "cc/backend/sound/sound_controller.h"

namespace backend {
namespace memory {

using std::unique_ptr;
using graphics::GraphicsController;
using graphics::Screen;
using timer::TimerModule;
using sound::SoundController;

Memory::Memory() = default;
Memory::~Memory() = default;

void Memory::Init(uint8_t* rom, size_t length, Screen* screen) {
  memory_mapper_ = unique_ptr<MemoryMapper>(new MemoryMapper());

  unimplemented_module_ = unique_ptr<UnimplementedModule>(new UnimplementedModule());
  unimplemented_module_->Init();
  memory_mapper_->RegisterModule(*unimplemented_module_);

  primary_flags_ = unique_ptr<PrimaryFlags>(new PrimaryFlags());
  primary_flags_->Init();
  memory_mapper_->RegisterModule(*primary_flags_);

  default_module_ = unique_ptr<DefaultModule>(new DefaultModule());
  default_module_->Init();
  memory_mapper_->RegisterModule(*default_module_);

  dma_transfer_module_ = unique_ptr<DMATransferModule>(new DMATransferModule());
  dma_transfer_module_->Init(memory_mapper_.get());
  memory_mapper_->RegisterModule(*dma_transfer_module_);

  joypad_module_ = unique_ptr<JoypadModule>(new JoypadModule());
  joypad_module_->Init(primary_flags_->interrupt_flag());
  memory_mapper_->RegisterModule(*joypad_module_);

  timer_module_ = unique_ptr<TimerModule>(new TimerModule());
  timer_module_->Init(primary_flags_->interrupt_flag());
  memory_mapper_->RegisterModule(*timer_module_);

  mbc_module_ = unique_ptr<MBCModule>(new MBCModule());
  mbc_module_->Init(rom, length);
  memory_mapper_->RegisterModule(*mbc_module_);

  graphics_controller_ = unique_ptr<GraphicsController>(new GraphicsController(screen, primary_flags_.get()));
  graphics_controller_->Init();
  memory_mapper_->RegisterModule(*graphics_controller_);

  sound_controller_ = unique_ptr<SoundController>(new SoundController());
  sound_controller_->Init();
  memory_mapper_->RegisterModule(*sound_controller_);
}

void Memory::Tick(int ticks) {
  timer_module_->Tick(ticks);
  graphics_controller_->Tick(ticks);
  sound_controller_->Tick(ticks);
}

} // namespace memory
} // namespace backend
