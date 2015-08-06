#ifndef TURBO_SANTA_COMMON_BACK_END_MEMORY_DMA_TRANSFER_H_
#define TURBO_SANTA_COMMON_BACK_END_MEMORY_DMA_TRANSFER_H_

#include <memory>
#include "cc/backend/memory/flags.h"
#include "cc/backend/memory/memory_mapper.h"
#include "cc/backend/memory/module.h"

namespace back_end {
namespace memory {

class DMATransferFlag : public Flag {
 public:
  DMATransferFlag(MemoryMapper* mapper) : Flag(0xff46), mapper_(mapper) {}

  unsigned char Read(unsigned short) { return 0xff; }

  void Write(unsigned short, unsigned char value) {
    // TODO(Brendan): This should actually take the expected amount of time.
    // This should also disable all memory outside of high RAM.
    unsigned short source_address = value * 0x100;
    unsigned short destination_address = 0xfe00;
    while (destination_address <= 0xfe9f) {
      unsigned char value = mapper_->Read(source_address);
      mapper_->Write(destination_address, value);
      source_address++;
      destination_address++;
    }
  }

 private:
  MemoryMapper* mapper_;
};

class DMATransferModule : public Module {
 public:
  void Init(MemoryMapper* memory_mapper) {
    flag_ = std::unique_ptr<DMATransferFlag>(new DMATransferFlag(memory_mapper));
    add_flag(flag_.get());
  }

 private:
  std::unique_ptr<DMATransferFlag> flag_;
};

} // namespace memory
} // namespace back_end

#endif // TURBO_SANTA_COMMON_BACK_END_MEMORY_DMA_TRANSFER_H_
