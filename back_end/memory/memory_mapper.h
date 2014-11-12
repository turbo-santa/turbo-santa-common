#ifndef TURBO_SANTA_COMMON_BACK_END_MEMORY_MEMORY_MAPPER_H_
#define TURBO_SANTA_COMMON_BACK_END_MEMORY_MEMORY_MAPPER_H_

#include <memory>

#include "back_end/memory/mbc.h"
#include "back_end/memory/memory_segment.h"

namespace back_end {
namespace memory {

class MemoryMapper {
 public:
  MemoryMapper() : mbc_(CreateNoMBC({0x00}, 1)) {}

  virtual unsigned char Read(unsigned short address);

  virtual void Write(unsigned short address, unsigned char value);

 private:
  std::unique_ptr<MBC> mbc_; // rom_bank_0    0x0000 - 0x3fff
                             // rom_bank_n    0x4000 - 0x7fff
                             // cartridge_ram 0xa000 - 0xbfff
  std::unique_ptr<NullMemorySegment> rom_bank_0_ = std::unique_ptr<NullMemorySegment>(new NullMemorySegment(0x0000, 0x3fff));
  std::unique_ptr<NullMemorySegment> rom_bank_n_ = std::unique_ptr<NullMemorySegment>(new NullMemorySegment(0x4000, 0x7fff));
  std::unique_ptr<NullMemorySegment> video_ram_ = std::unique_ptr<NullMemorySegment>(new NullMemorySegment(0x8000, 0x9fff));
  std::unique_ptr<NullMemorySegment> cartridge_ram_ = std::unique_ptr<NullMemorySegment>(new NullMemorySegment(0xa000, 0xbfff));
  std::unique_ptr<NullMemorySegment> internal_ram_ = std::unique_ptr<NullMemorySegment>(new NullMemorySegment(0xc000, 0xdfff));
  std::unique_ptr<NullMemorySegment> echo_ram_ = std::unique_ptr<NullMemorySegment>(new NullMemorySegment(0xe000, 0xfdff));
  std::unique_ptr<NullMemorySegment> sprite_attribute_table_ = std::unique_ptr<NullMemorySegment>(new NullMemorySegment(0xfe00, 0xfe9f));
  std::unique_ptr<NullMemorySegment> not_usable_ = std::unique_ptr<NullMemorySegment>(new NullMemorySegment(0xfea0, 0xfeff));
  std::unique_ptr<NullMemorySegment> io_ports_ = std::unique_ptr<NullMemorySegment>(new NullMemorySegment(0xff00, 0xff7f));
  std::unique_ptr<NullMemorySegment> high_ram_ = std::unique_ptr<NullMemorySegment>(new NullMemorySegment(0xff80, 0xfffe));
  std::unique_ptr<NullMemorySegment> interupt_enable_register_ = std::unique_ptr<NullMemorySegment>(new NullMemorySegment(0xffff, 0xffff));
};

} // namespace memory
} // namespace back_end
#endif // TURBO_SANTA_COMMON_BACK_END_MEMORY_MEMORY_MAPPER_H_
