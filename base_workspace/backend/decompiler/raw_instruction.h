#ifndef TURBO_SANTA_COMMON_BACKEND_RAW_INSTRUCTION_H_
#define TURBO_SANTA_COMMON_BACKEND_RAW_INSTRUCTION_H_

#include <cstdint>
#include <array>
#include "backend/decompiler/rom_bridge.h"
#include "submodules/glog/src/glog/logging.h"

namespace back_end {
namespace decompiler {

enum class ValueWidth {
  BIT_0,
  BIT_8,
  BIT_16,
  BIT_24,
};

uint16_t to_width_bytes(ValueWidth width);

ValueWidth operator+(ValueWidth left, ValueWidth right);

class RawInstructionBase {
 public:
  virtual uint16_t instr(ValueWidth width) const = 0;

  virtual uint16_t param(ValueWidth width) const = 0;

  virtual uint8_t instr_8bit() const = 0;

  virtual uint16_t instr_16bit() const = 0; 

  virtual uint8_t param_8bit() const = 0;

  virtual uint16_t param_16bit() const = 0;
};

template <uint8_t LEN>
class RawInstruction : public RawInstructionBase {
 public:
  void set_data(const ROMBridge& bridge, uint16_t start_address) {
    for (size_t i = 0; i < value_.size(); i++) {
      value_[i] = bridge.at(i + start_address);
    }
  }

  virtual uint16_t instr(ValueWidth width) const {
    switch (width) {
      case ValueWidth::BIT_8:
        return instr_8bit();
      case ValueWidth::BIT_16:
        return instr_16bit();
      default:
        LOG(FATAL) << "Data value widths other than 8 bit and 16 bit are invalid";
    }
  }

  virtual uint16_t param(ValueWidth width) const {
    switch (width) {
      case ValueWidth::BIT_8:
        return param_8bit();
      case ValueWidth::BIT_16:
        return param_16bit();
      default:
        LOG(FATAL) << "Data value widths other than 8 bit and 16 bit are invalid";
    }
  }

  uint8_t instr_8bit() const { 
    return value_[0];
  }

  uint16_t instr_16bit() const { 
    return (static_cast<uint16_t>(value_[1]) << 8) + value_[0];
  }

  uint8_t param_8bit() const { 
    return value_[LEN - 1];
  }

  uint16_t param_16bit() const { 
    return (static_cast<uint16_t>(value_[LEN - 1]) << 8) + value_[LEN - 2];
  }

 protected:
  std::array<uint8_t, LEN> value_;
};

class RawInstruction8Bit : public RawInstruction<1> {
 public:
  uint16_t instr_16bit() const {
    LOG(FATAL) << "Instruction is only 8 bits wide.";
  }

  uint8_t param_8bit() const { 
    LOG(FATAL) << "Instruction is only 8 bits wide, cannot have 8 bit param.";
  }

  uint16_t param_16bit() const { 
    LOG(FATAL) << "Instruction is only 8 bits wide, cannot have 16 bit param.";
  }
};

class RawInstruction16Bit : public RawInstruction<2> {
 public:
  uint16_t param_16bit() const { 
    LOG(FATAL) << "Instruction is only 16 bits wide, cannot have 16 bit param.";
  }
};

class RawInstruction24Bit : public RawInstruction<3> {};

} // namespace decompiler
} // namespace decompiler

#endif // TURBO_SANTA_COMMON_BACKEND_RAW_INSTRUCTION_H_
