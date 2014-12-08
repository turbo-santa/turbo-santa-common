#ifndef TURBO_SANTA_COMMON_BACK_END_MEMORY_MEMORY_MAPPER_H_
#define TURBO_SANTA_COMMON_BACK_END_MEMORY_MEMORY_MAPPER_H_

#include "back_end/config.h"

#include <memory>

#include "back_end/debugger/deltas.h"
#include "back_end/graphics/dma_transfer.h"
#include "back_end/graphics/graphics_flags.h"
#include "back_end/memory/echo_segment.h"
#include "back_end/memory/internal_rom.h"
#include "back_end/memory/interrupt_flag.h"
#include "back_end/memory/mbc.h"
#include "back_end/memory/memory_segment.h"
#include "back_end/memory/ram_segment.h"
#include "back_end/memory/vram_segment.h"
#include "back_end/memory/joypad_memory.h"

namespace test_harness {
class TestHarness;
} // namespace test_harness

namespace back_end {
namespace clocktroller {
class ClocktrollerTest;
} // namespace back_end
} // namespace clocktroller

namespace back_end {
namespace graphics {
class DMATransfer;
} // namespace graphics
} // namespace back_end

namespace back_end {
namespace memory {

class MemoryMapper {
 public:
  MemoryMapper();
  MemoryMapper(unsigned char* rom, long size, bool use_internal_rom = false);

  virtual unsigned char Read(unsigned short address);

  virtual void Write(unsigned short address, unsigned char value);

  void Enable();

  void Disable();

  InternalROMFlag* internal_rom_flag() { return internal_rom_flag_.get(); }
  InterruptFlag* interrupt_flag() { return interrupt_flag_.get(); }
  InterruptEnable* interrupt_enable() { return interrupt_enable_.get(); }
  graphics::GraphicsFlags* graphics_flags() { return graphics_flags_.get(); }
  VRAMSegment* vram_segment() { return video_ram_.get(); }
  OAMSegment* oam_segment() { return sprite_attribute_table_.get(); }
  debugger::MemoryProducer* memory_producer() { return &memory_producer_; }

  static const int kMaxSize = 0x10000;

 private:
  std::unique_ptr<InternalROM> internal_rom_ = std::unique_ptr<InternalROM>(new InternalROM()); // 0x0000 - 0x0100
  std::unique_ptr<MBC> mbc_; // rom_bank_0    0x0000 - 0x3fff
                             // rom_bank_n    0x4000 - 0x7fff
                             // cartridge_ram 0xa000 - 0xbfff
  std::unique_ptr<VRAMSegment> video_ram_ = std::unique_ptr<VRAMSegment>(new VRAMSegment()); // 0x8000 - 0x9fff
  std::unique_ptr<RAMSegment> internal_ram_0_ = std::unique_ptr<RAMSegment>(new RAMSegment(0xc000, 0xcfff));
  // TODO(Brendan): In Gameboy Color mode (and GBC mode only) internal_ram_1_
  // should be banked and switchable similiar to how the MBCN are done.
  std::unique_ptr<RAMSegment> internal_ram_1_ = std::unique_ptr<RAMSegment>(new RAMSegment(0xd000, 0xdfff));
  std::unique_ptr<EchoSegment> echo_ram_ = std::unique_ptr<EchoSegment>(new EchoSegment(internal_ram_0_.get(), internal_ram_1_.get())); // 0xe000 - 0xfdff
  std::unique_ptr<OAMSegment> sprite_attribute_table_ = std::unique_ptr<OAMSegment>(new OAMSegment()); // 0xfe00 - 0xfe9f
  std::unique_ptr<NullMemorySegment> not_usable_ = std::unique_ptr<NullMemorySegment>(new NullMemorySegment(0xfea0, 0xfeff));
  std::unique_ptr<InterruptFlag> interrupt_flag_ = std::unique_ptr<InterruptFlag>(new InterruptFlag()); // 0xff0f
  std::unique_ptr<graphics::GraphicsFlags> graphics_flags_ = std::unique_ptr<graphics::GraphicsFlags>(new graphics::GraphicsFlags());
  std::unique_ptr<graphics::DMATransfer> dma_transfer_ = std::unique_ptr<graphics::DMATransfer>(new graphics::DMATransfer(this));
  std::unique_ptr<InternalROMFlag> internal_rom_flag_ = std::unique_ptr<InternalROMFlag>(new InternalROMFlag()); // 0xff50
  std::unique_ptr<JoypadMemory> joypad_ = std::unique_ptr<JoypadMemory>(new JoypadMemory()); // 0xff00
  std::unique_ptr<NullMemorySegment> io_ports_ = std::unique_ptr<NullMemorySegment>(new NullMemorySegment(0xff01, 0xff7f));
  std::unique_ptr<RAMSegment> high_ram_ = std::unique_ptr<RAMSegment>(new RAMSegment(0xff80, 0xfffe));
  std::unique_ptr<InterruptEnable> interrupt_enable_ = std::unique_ptr<InterruptEnable>(new InterruptEnable()); // 0xffff

  debugger::MemoryProducer memory_producer_;

  friend class test_harness::TestHarness;
  friend class back_end::clocktroller::ClocktrollerTest;
};

class DMATransfer : public SingleAddressSegment {
 public:
  DMATransfer(MemoryMapper* mapper) : SingleAddressSegment(0xff46), mapper_(mapper) {}

  virtual unsigned char Read(unsigned short) { return 0xff; }

  virtual void Write(unsigned short, unsigned char value) {
    mapper_->Disable();

    // TODO(Brendan): This should actually take the expected amount of time.
    unsigned short source_address = value * 0x100;
    unsigned short destination_address = 0xfe00;
    while (destination_address <= 0xfe9f) {
      unsigned char value = mapper_->Read(source_address);
      mapper_->Write(destination_address, value);
      source_address++;
      destination_address++;
    }

    mapper_->Enable();
  }

 private:
  MemoryMapper* mapper_;
};


} // namespace memory
} // namespace back_end
#endif // TURBO_SANTA_COMMON_BACK_END_MEMORY_MEMORY_MAPPER_H_
