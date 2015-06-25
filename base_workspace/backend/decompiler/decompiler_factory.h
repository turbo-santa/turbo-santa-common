#ifndef TURBO_SANTA_COMMON_BACKEND_DECOMPILER_FACTORY_H_
#define TURBO_SANTA_COMMON_BACKEND_DECOMPILER_FACTORY_H_

#include <cstdint>
#include <functional>
#include <memory>
#include <vector>
#include "backend/decompiler/decompiler.h"
#include "backend/decompiler/rom_bridge.h"

namespace back_end {
namespace decompiler {

class DecompilerFactory {
 public:
  enum DecompilerType {
    FORMATTED_ROM, // Most ROMs.
    UNFORMATTED_ROM,
    NONE,
  };

  std::unique_ptr<Decompiler> Build() {
    uint8_t rom_type = 0x00;
    if (decompiler_type_ == FORMATTED_ROM) {
      rom_type = rom_->at(0x0147);
    }
    Decompiler* decompiler = new Decompiler(*rom_, rom_type);
    switch (decompiler_type_) {
      case FORMATTED_ROM:
        // Restart vectors.
        decompiler->AddPathStart(0x0000);
        decompiler->AddPathStart(0x0008);
        decompiler->AddPathStart(0x0010);
        decompiler->AddPathStart(0x0018);
        decompiler->AddPathStart(0x0020);
        decompiler->AddPathStart(0x0028);
        decompiler->AddPathStart(0x0030);
        decompiler->AddPathStart(0x0038);
        // Interrupt vectors.
        decompiler->AddPathStart(0x0040);
        decompiler->AddPathStart(0x0048);
        decompiler->AddPathStart(0x0050);
        decompiler->AddPathStart(0x0058);
        decompiler->AddPathStart(0x0060);
        // Actual start.
        decompiler->AddPathStart(0x0100);
        break;
      case UNFORMATTED_ROM:
        decompiler->AddPathStart(0x0000);
        break;
      case NONE:
        // Do not add any instructions.
        break;
    }
    return std::unique_ptr<Decompiler>(decompiler);
  }

  void set_rom(const ROMBridge* rom) { rom_ = rom; }
  void set_type(DecompilerType type) { decompiler_type_ = type; }

 private:
  const ROMBridge* rom_;
  DecompilerType decompiler_type_;
};

} // namespace decompiler
} // namespace decompiler

#endif // TURBO_SANTA_COMMON_BACKEND_DECOMPILER_FACTORY_H_
