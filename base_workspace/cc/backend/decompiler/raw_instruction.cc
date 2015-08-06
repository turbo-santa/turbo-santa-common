#include "cc/backend/decompiler/raw_instruction.h"

namespace back_end {
namespace decompiler {

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
          return ValueWidth::BIT_8;
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
          return ValueWidth::BIT_16;
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

} // namespace decompiler
} // namespace decompiler
