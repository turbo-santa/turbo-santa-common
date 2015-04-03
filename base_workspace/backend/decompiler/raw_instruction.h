#ifndef TURBO_SANTA_COMMON_BACKEND_RAW_INSTRUCTION_H_
#define TURBO_SANTA_COMMON_BACKEND_RAW_INSTRUCTION_H_

#include "backend/config.h"

#include <cstdint>
#include <array>
#include "submodules/glog/src/glog/logging.h"

namespace back_end {
namespace decompiler {

enum class ValueWidth {
  BIT_0,
  BIT_8,
  BIT_16,
  BIT_24,
};

uint16_t to_width_bytes(ValueWidth width) {
  switch (width) {
    case ValueWidth::BIT_0:
      return 0;
    case ValueWidth::BIT_8:
      return 1;
    case ValueWidth::BIT_16:
      return 2;
    case ValueWidth::BIT_24:
      return 3;
  }
}

ValueWidth operator+(ValueWidth left, ValueWidth right) {
  switch (left) {
    case ValueWidth::BIT_0:
      return right;
    case ValueWidth::BIT_8:
      switch (right) {
        case ValueWidth::BIT_0:
          return ValueWidth::BIT_0;
        case ValueWidth::BIT_8:
          return ValueWidth::BIT_16;
        case ValueWidth::BIT_16:
          return ValueWidth::BIT_24;
        case ValueWidth::BIT_24:
          LOG(FATAL) << "No value widths larger than 24 are supported.";
      }
    case ValueWidth::BIT_16:
      switch (right) {
        case ValueWidth::BIT_0:
          return ValueWidth::BIT_8;
        case ValueWidth::BIT_8:
          return ValueWidth::BIT_24;
        case ValueWidth::BIT_16:
          LOG(FATAL) << "No value widths larger than 24 are supported.";
        case ValueWidth::BIT_24:
          LOG(FATAL) << "No value widths larger than 24 are supported.";
      }
    case ValueWidth::BIT_24:
      switch (right) {
        case ValueWidth::BIT_0:
          return ValueWidth::BIT_24;
        case ValueWidth::BIT_8:
          LOG(FATAL) << "No value widths larger than 24 are supported.";
        case ValueWidth::BIT_16:
          LOG(FATAL) << "No value widths larger than 24 are supported.";
        case ValueWidth::BIT_24:
          LOG(FATAL) << "No value widths larger than 24 are supported.";
      }
  }
}

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
  void set_ptr(uint8_t* ptr) {
    for (size_t i = 0; i < value_.size(); i++) {
      value_[i] = ptr[i];
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
