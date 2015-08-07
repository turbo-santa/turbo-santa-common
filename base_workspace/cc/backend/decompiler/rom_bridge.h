#ifndef TURBO_SANTA_COMMON_BACKEND_ROM_BRIDGE_H_
#define TURBO_SANTA_COMMON_BACKEND_ROM_BRIDGE_H_

#include <cstddef>
#include <cstdint>
#include <vector>

namespace backend {
namespace decompiler {

class ROMBridge {
 public:
  virtual uint8_t at(uint16_t address) const = 0;
  virtual uint16_t min() const = 0;
  virtual uint16_t max() const = 0;

  virtual bool in_range(uint16_t address) const { 
    return min() <= address && address <= max(); 
  }
};

class VectorROMBridge : public ROMBridge {
 public:
  VectorROMBridge(std::vector<uint8_t>* data) : data_(data) {}

  uint8_t at(uint16_t address) const { return data_->at(address); }

  uint16_t min() const { return 0; }

  uint16_t max() const { return static_cast<uint16_t>(data_->size() - 1); }

 private:
  std::vector<uint8_t>* data_;
};

} // namespace decompiler
} // namespace backend

#endif // TURBO_SANTA_COMMON_BACKEND_ROM_BRIDGE_H_
