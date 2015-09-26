#ifndef TURBO_SANTA_COMMON_BACK_END_MEMORY_MEMORY_H_
#define TURBO_SANTA_COMMON_BACK_END_MEMORY_MEMORY_H_

#include <cstdint>
#include <memory>
#include "cc/backend/memory/joypad/joypad_module.h"
#include "cc/backend/memory/mbc/mbc_module.h"

namespace backend {
namespace graphics {
class GraphicsController;
class Screen;
} // namespace graphics

namespace memory {
class MemoryMapper;
class PrimaryFlags;
class DefaultModule;
class DMATransferModule;
class UnimplementedModule;

namespace timer {
class TimerModule;
} // namespace timer

} // namespace memory

namespace sound {
class SoundController;
class SoundPlayer;
} // namespace sound
} // namespace backend

namespace backend {
namespace memory {
// Initializes all modules residing under the memory namespace.
class Memory {
 public:
  Memory();
  ~Memory();

  void Init(uint8_t* rom, size_t length, graphics::Screen* screen, sound::SoundPlayer* sound_player);

  void Tick(int ticks);

  MemoryMapper* memory_mapper() { return memory_mapper_.get(); }

  JoypadFlag* joypad_flag() { return joypad_module_->joypad_flag(); }

  PrimaryFlags* primary_flags() { return primary_flags_.get(); }

  Flag* internal_rom_flag() { return mbc_module_->internal_rom_flag(); }

 private:
  std::unique_ptr<MemoryMapper> memory_mapper_;
  std::unique_ptr<graphics::GraphicsController> graphics_controller_;
  std::unique_ptr<PrimaryFlags> primary_flags_;
  std::unique_ptr<DefaultModule> default_module_;
  std::unique_ptr<JoypadModule> joypad_module_;
  std::unique_ptr<timer::TimerModule> timer_module_;
  std::unique_ptr<MBCModule> mbc_module_;
  std::unique_ptr<DMATransferModule> dma_transfer_module_;
  std::unique_ptr<sound::SoundController> sound_controller_;
  std::unique_ptr<UnimplementedModule> unimplemented_module_;
};

} // namespace memory
} // namespace backend

#endif // TURBO_SANTA_COMMON_BACK_END_MEMORY_MEMORY_H_
