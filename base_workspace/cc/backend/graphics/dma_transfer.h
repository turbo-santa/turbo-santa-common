#ifndef TURBO_SANTA_COMMON_BACK_END_GRAPHICS_DMA_TRANSFER_H_
#define TURBO_SANTA_COMMON_BACK_END_GRAPHICS_DMA_TRANSFER_H_

#include "cc/backend/memory/memory_segment.h"

namespace back_end {
namespace memory {
class MemoryMapper;
} // namespace memory
} // namespace back_end

namespace back_end {
namespace graphics {

class DMATransfer : public memory::SingleAddressSegment {
 public:
  DMATransfer(memory::MemoryMapper* mapper) : SingleAddressSegment(0xff46), mapper_(mapper) {}

  virtual unsigned char Read(unsigned short) { return 0xff; }

  virtual void Write(unsigned short address, unsigned char value);

 private:
  memory::MemoryMapper* mapper_;
};

} // namespace graphics
} // namespace back_end

#endif // TURBO_SANTA_COMMON_BACK_END_GRAPHICS_DMA_TRANSFER_H_
