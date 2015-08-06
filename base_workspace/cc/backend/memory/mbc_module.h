#ifndef TURBO_SANTA_COMMON_BACK_END_MEMORY_MBC_MODULE_H_
#define TURBO_SANTA_COMMON_BACK_END_MEMORY_MBC_MODULE_H_

#include <memory>
#include "cc/backend/memory/internal_rom.h"
#include "cc/backend/memory/mbc.h"
#include "cc/backend/memory/memory_segment.h"
#include "cc/backend/memory/module.h"

namespace back_end {
namespace memory {

class MBCWrapper : public MemorySegment {
 public:
  void Init(unsigned char* program_rom, long size) {
    mbc_ = ConstructMBC(program_rom, size);
  }

  unsigned char Read(unsigned short address) {
    if (!internal_rom_flag_.is_set() && internal_rom_.InRange(address)) {
      return internal_rom_.Read(address);
    } else {
      return mbc_->Read(address);
    }
  }

  void Write(unsigned short address, unsigned char value) {
    if (!internal_rom_flag_.is_set() && internal_rom_.InRange(address)) {
      internal_rom_.Write(address, value);
    } else {
      mbc_->Write(address, value);
    }
  }

  void ForceWrite(unsigned short address, unsigned char value) {
    if (!internal_rom_flag_.is_set() && internal_rom_.InRange(address)) {
      internal_rom_.ForceWrite(address, value);
    } else {
      mbc_->ForceWrite(address, value);
    }
  }

  bool InRange(unsigned short address) { return mbc_->InRange(address); }

  Flag* internal_rom_flag() { return &internal_rom_flag_; }

 private:
  InternalROM internal_rom_;
  InternalROMFlag internal_rom_flag_;
  std::unique_ptr<MBC> mbc_;
};

class MBCModule : public Module {
 public:
  void Init(unsigned char* program_rom, long size) {
    mbc_.Init(program_rom, size);
    add_memory_segment(&mbc_);
    add_flag(internal_rom_flag());
  }

  Flag* internal_rom_flag() { return mbc_.internal_rom_flag(); }

 private:
  MBCWrapper mbc_;
};

} // namespace memory
} // namespace back_end
#endif // TURBO_SANTA_COMMON_BACK_END_MEMORY_MBC_MODULE_H_
