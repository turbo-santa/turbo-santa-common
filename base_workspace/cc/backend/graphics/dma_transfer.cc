#include "cc/backend/graphics/dma_transfer.h"

#include "cc/backend/memory/memory_mapper.h"
#include "submodules/glog/src/glog/logging.h"

namespace backend {
namespace graphics {

void DMATransfer::Write(unsigned short, unsigned char value) {
  LOG(INFO) << "Starting DMA transfer sequence with value " << std::hex << value + 0x0000;
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
  LOG(INFO) << "Finished DMA transfer sequence.";
}

} // namespace graphics
} // namespace backend
