#ifndef TURBO_SANTA_COMMON_BACK_END_MEMORY_DEFAULT_MODULE_H_
#define TURBO_SANTA_COMMON_BACK_END_MEMORY_DEFAULT_MODULE_H_

#include <memory>

#include "cc/backend/memory/memory_segment.h"
#include "cc/backend/memory/module.h"
#include "cc/backend/memory/ram/echo_segment.h"
#include "cc/backend/memory/ram/ram_segment.h"

// The majority of memory segments are simple and do not require any special
// initialization; they have been collected here.
//
// Other more complicated memory segments have been divested into different
// modules.
//
// The overall memory layout:
//                   0x0000 - 0x0100
// rom_bank_0        0x0000 - 0x3fff
// rom_bank_n        0x4000 - 0x7fff
// cartridge_ram     0xa000 - 0xbfff
// vram              0x8000 - 0x9fff
// ram_bank_0        0xc000 - 0xcfff
// ram_bank_n        0xd000 - 0xdfff
// echo              0xe000 - 0xfdff
// oam               0xfe00 - 0xfe9f
// not_usable        0xfea0 - 0xfeff
// interrupt_flag    0xff0f
// 
//
// internal_rom_flag 0xff50
// joypad            0xff00
// io_ports          0xff01 - 0xff7f
// high_ram          0xff80 - 0xfffe
// interrupt_enable  0xffff

namespace backend {
namespace memory {

class DefaultModule : public Module {
 public:
  void Init() {
    add_memory_segment(&internal_ram_0_);
    add_memory_segment(&internal_ram_1_);
    add_memory_segment(&echo_segment_);
    add_memory_segment(&not_usable_);
    add_memory_segment(&high_ram_);
  }

 private:
  // InternalROM internal_rom_;                                                //                   0x0000 - 0x0100
  // std::unique_ptr<MBC> mbc_;                                                // rom_bank_0        0x0000 - 0x3fff
                                                                               // rom_bank_n        0x4000 - 0x7fff
                                                                               // cartridge_ram     0xa000 - 0xbfff
  // VRAMSegment vram_;                                                        // vram              0x8000 - 0x9fff
  RAMSegment internal_ram_0_ = RAMSegment(0xc000, 0xcfff);                     // ram_bank_0        0xc000 - 0xcfff
  RAMSegment internal_ram_1_ = RAMSegment(0xd000, 0xdfff);                     // ram_bank_n        0xd000 - 0xdfff
  EchoSegment echo_segment_ = EchoSegment(&internal_ram_0_, &internal_ram_1_); // echo              0xe000 - 0xfdff
  // OAMSegment oam_segment_;                                                  // oam               0xfe00 - 0xfe9f
  NullMemorySegment not_usable_ = NullMemorySegment(0xfea0, 0xfeff);           // not_usable        0xfea0 - 0xfeff
  // InterruptFlag interrupt_flag_;                                            // interrupt_flag    0xff0f
  // GraphicsFlags graphics_flags_;                                            // 
  // DMATransfer dma_transfer_;                                                //
  // InternalROMFlag internal_rom_flag_;                                       // internal_rom_flag 0xff50
  // JoypadMemory joypad_;                                                     // joypad            0xff00
  // NullMemorySegment io_ports_ = NullMemorySegment(0xff01, 0xff7f);          // io_ports          0xff01 - 0xff7f
  RAMSegment high_ram_ = RAMSegment(0xff80, 0xfffe);                           // high_ram          0xff80 - 0xfffe
  // InterruptEnable interrupt_enable_;                                        // interrupt_enable  0xffff
};

} // namespace memory
} // namespace backend

#endif // TURBO_SANTA_COMMON_BACK_END_MEMORY_DEFAULT_MODULE_H_
