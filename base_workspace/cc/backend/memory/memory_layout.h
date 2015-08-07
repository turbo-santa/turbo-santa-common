#ifndef TURBO_SANTA_COMMON_BACK_END_MEMORY_MEMORY_LAYOUT_H_
#define TURBO_SANTA_COMMON_BACK_END_MEMORY_MEMORY_LAYOUT_H_

#include <cstdint>

namespace backend {
namespace memory {

static const uint16_t kInterruptEnableRegister = 0xffff;
static const uint16_t kHighRAMMax = 0xfffe;
static const uint16_t kHighRAMMin = 0xff80;
static const uint16_t kFlagMax = 0xff7f;
static const uint16_t kFlagMin = 0xff00;
static const uint16_t kNotUsableMax = 0xfeff;
static const uint16_t kNotUsableMin = 0xfea0;
static const uint16_t kOAMMax = 0xfe9f;
static const uint16_t kOAMMin = 0xfe00;
static const uint16_t kECHOMax = 0xfdff;
static const uint16_t kECHOMin = 0xe000;
static const uint16_t kWRAMBank1Max = 0xdfff;
static const uint16_t kWRAMBank1Min = 0xd000;
static const uint16_t kWRAMBank0Max = 0xcfff;
static const uint16_t kWRAMBank0Min = 0xc000;
static const uint16_t kExternalRAMMax = 0xbfff;
static const uint16_t kExternalRAMMin = 0xa000;
static const uint16_t kVRAMMax = 0x9fff;
static const uint16_t kVRAMMin = 0x8000;
static const uint16_t kROMBankNMax = 0x7fff;
static const uint16_t kROMBankNMin = 0x4000;
static const uint16_t kROMBank0Max = 0x3fff;
static const uint16_t kROMBank0Min = 0x0000;

class MemoryAddressRange {
 public:
  virtual uint16_t max() = 0;
  virtual uint16_t min() = 0;
};

} // namespace memory
} // namespace backend

#endif // TURBO_SANTA_COMMON_BACK_END_MEMORY_MEMORY_LAYOUT_H_
