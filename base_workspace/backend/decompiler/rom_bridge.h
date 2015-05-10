#ifndef TURBO_SANTA_COMMON_BACKEND_ROM_BRIDGE_H_
#define TURBO_SANTA_COMMON_BACKEND_ROM_BRIDGE_H_

#include <cstddef>
#include <cstdint>
#include <vector>

namespace back_end {
namespace decompiler {

class ROMBridge {
 public:
  virtual uint8_t at(uint16_t address) const = 0;

  virtual size_t size() const = 0;
};

class VectorROMBridge : public ROMBridge {
 public:
  VectorROMBridge(std::vector<uint8_t>* data) : data_(data) {}

  virtual uint8_t at(uint16_t address) const { return data_->at(address); }

  virtual size_t size() const { return data_->size(); }

 private:
  std::vector<uint8_t>* data_;
};

} // namespace decompiler
} // namespace decompiler

#endif // TURBO_SANTA_COMMON_BACKEND_ROM_BRIDGE_H_
