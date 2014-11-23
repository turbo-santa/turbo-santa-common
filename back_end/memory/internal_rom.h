#ifndef TURBO_SANTA_COMMON_BACK_END_MEMORY_INTERNAL_ROM_H_
#define TURBO_SANTA_COMMON_BACK_END_MEMORY_INTERNAL_ROM_H_

#include "back_end/config.h"

#include <memory>
#include <vector>
#include <errno.h>
#include <stdio.h>
#include <glog/logging.h>

#include "back_end/memory/flags.h"
#include "back_end/memory/memory_segment.h"

namespace back_end {
namespace memory {

class InternalROM : public ContiguousMemorySegment {
 public:
  virtual unsigned char Read(unsigned short address) { return data_[address - lower_address_bound()]; }

  virtual void Write(unsigned short, unsigned char) {}

  static const int kInternalROMSize = 256;

  static std::unique_ptr<InternalROM> ConstructInternalROM() {
    InternalROM* rom = new InternalROM();

    FILE* file = fopen("DMG_ROM.bin", "r");
    if (file == nullptr) {
      LOG(FATAL) << "Could not open file: " << strerror(errno);
    }
    for (int i = 0; i < kInternalROMSize; i++) {
      unsigned char value = static_cast<unsigned char>(fgetc(file));
      rom->data_[i] = value;
    }
    fclose(file);

    return std::unique_ptr<InternalROM>(rom);
  }

 protected:
  unsigned short lower_address_bound() { return 0; }
  unsigned short upper_address_bound() { return kInternalROMSize - 1; }

 private:
  InternalROM() : data_(kInternalROMSize, 0x00) {}

  std::vector<unsigned char> data_;
};

class InternalROMFlag : public Flag {
 public:
  InternalROMFlag() : Flag(0xff50) {}

  virtual void Write(unsigned short address, unsigned char value) {
    if (!is_set()) {
      Flag::Write(address, value);
    }
  }

  bool is_set() { return flag() != 0; }
  void set() { set_flag(0x01); }
};

} // namespace memory
} // namespace back_end

#endif // TURBO_SANTA_COMMON_BACK_END_MEMORY_INTERNAL_ROM_H_
