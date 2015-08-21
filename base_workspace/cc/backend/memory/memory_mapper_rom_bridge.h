#ifndef TURBO_SANTA_COMMON_BACK_END_MEMORY_MEMORY_MAPPER_ROM_BRIDGE_H_
#define TURBO_SANTA_COMMON_BACK_END_MEMORY_MEMORY_MAPPER_ROM_BRIDGE_H_

#include <cstdint>
#include "cc/backend/decompiler/rom_bridge.h"
#include "cc/backend/memory/mbc/mbc.h"
#include "cc/backend/memory/memory_mapper.h"
#include "cc/backend/memory/memory_layout.h"

namespace backend {
namespace memory {

class MemoryMapperROMBridge : public decompiler::ROMBridge {
 public:
  MemoryMapperROMBridge(const MemoryMapper& data) : data_(data) {}

  uint8_t at(uint16_t address) const override {
    // TODO(Brendan): Make .Read(...) a constant method.
    return const_cast<MemoryMapper&>(data_).Read(address);
  }

  uint16_t min() const override { return 0; }

  uint16_t max() const override { return MBC::kROMMaxAddress; }

 private:
  const MemoryMapper& data_;
};

class MemoryMapperHighRAMBridge : public decompiler::ROMBridge {
 public:
  MemoryMapperHighRAMBridge(const MemoryMapper& data) : data_(data) {}

  uint8_t at(uint16_t address) const override {
    // TODO(Brendan): Make .Read(...) a constant method.
    return const_cast<MemoryMapper&>(data_).Read(address);
  }

  uint16_t min() const override { return memory::kHighRAMMin; }

  uint16_t max() const override { return memory::kHighRAMMax; }

 private:
  const MemoryMapper& data_;
};

} // namespace memory
} // namespace backend

#endif // TURBO_SANTA_COMMON_BACK_END_MEMORY_MEMORY_MAPPER_ROM_BRIDGE_H_
