#ifndef TURBO_SANTA_COMMON_BACK_END_MEMORY_MEMORY_MAPPER_ROM_BRIDGE_H_
#define TURBO_SANTA_COMMON_BACK_END_MEMORY_MEMORY_MAPPER_ROM_BRIDGE_H_

#include <cstdint>
#include "backend/decompiler/rom_bridge.h"
#include "backend/memory/mbc.h"
#include "backend/memory/memory_mapper.h"

namespace back_end {
namespace memory {

class MemoryMapperROMBridge : public decompiler::ROMBridge {
 public:
  MemoryMapperROMBridge(const MemoryMapper& data) : data_(data) {}

  uint8_t at(uint16_t address) const override {
    // TODO(Brendan): Make .Read(...) a constant method.
    return const_cast<MemoryMapper&>(data_).Read(address);
  }

  size_t size() const override { return MBC::kROMMaxAddress + 1; }

 private:
  const MemoryMapper& data_;
};

} // namespace memory
} // namespace back_end

#endif // TURBO_SANTA_COMMON_BACK_END_MEMORY_MEMORY_MAPPER_ROM_BRIDGE_H_
