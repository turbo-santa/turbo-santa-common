#ifndef TURBO_SANTA_COMMON_BACK_END_MEMORY_MEMORY_MAPPER_H_
#define TURBO_SANTA_COMMON_BACK_END_MEMORY_MEMORY_MAPPER_H_

#include <memory>

#include "back_end/memory/echo_segment.h"
#include "back_end/memory/mbc.h"
#include "back_end/memory/memory_segment.h"
#include "back_end/memory/ram_segment.h"

namespace test_harness {
class TestHarness;
} // namespace test_harness

namespace back_end {
namespace memory {

class MemoryMapper {
 public:
  MemoryMapper(); 

  virtual unsigned char Read(unsigned short address);

  virtual void Write(unsigned short address, unsigned char value);

  static const int kMaxSize = 0x10000;

 private:
  std::unique_ptr<MBC> mbc_; // rom_bank_0    0x0000 - 0x3fff
                             // rom_bank_n    0x4000 - 0x7fff
                             // cartridge_ram 0xa000 - 0xbfff
  std::unique_ptr<NullMemorySegment> video_ram_ = std::unique_ptr<NullMemorySegment>(new NullMemorySegment(0x8000, 0x9fff));
  std::unique_ptr<RAMSegment> internal_ram_0_ = std::unique_ptr<RAMSegment>(new RAMSegment(0xc000, 0xcfff));
  // TODO(Brendan): In Gameboy Color mode (and GBC mode only) internal_ram_1_
  // should be banked and switchable similiar to how the MBCN are done.
  std::unique_ptr<RAMSegment> internal_ram_1_ = std::unique_ptr<RAMSegment>(new RAMSegment(0xd000, 0xdfff));
  std::unique_ptr<EchoSegment> echo_ram_ = std::unique_ptr<EchoSegment>(new EchoSegment(internal_ram_0_.get(), internal_ram_1_.get())); // 0xe000 - 0xfdff
  std::unique_ptr<NullMemorySegment> sprite_attribute_table_ = std::unique_ptr<NullMemorySegment>(new NullMemorySegment(0xfe00, 0xfe9f));
  std::unique_ptr<NullMemorySegment> not_usable_ = std::unique_ptr<NullMemorySegment>(new NullMemorySegment(0xfea0, 0xfeff));
  std::unique_ptr<NullMemorySegment> io_ports_ = std::unique_ptr<NullMemorySegment>(new NullMemorySegment(0xff00, 0xff7f));
  std::unique_ptr<RAMSegment> high_ram_ = std::unique_ptr<RAMSegment>(new RAMSegment(0xff80, 0xfffe));
  std::unique_ptr<NullMemorySegment> interupt_enable_register_ = std::unique_ptr<NullMemorySegment>(new NullMemorySegment(0xffff, 0xffff));

  friend class test_harness::TestHarness;
};

} // namespace memory
} // namespace back_end
#endif // TURBO_SANTA_COMMON_BACK_END_MEMORY_MEMORY_MAPPER_H_
