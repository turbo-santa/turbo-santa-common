#ifndef TURBO_SANTA_COMMON_BACK_END_MEMORY_JOYPAD_MEMORY_H_
#define TURBO_SANTA_COMMON_BACK_END_MEMORY_JOYPAD_MEMORY_H_

#include "back_end/memory/memory_segment.h"

namespace back_end {
namespace memory {
class MemoryMapper;
}
}

namespace back_end {
namespace memory {
class JoypadMemory : public SingleAddressSegment {
 public:
    JoypadMemory(MemoryMapper*);
    virtual unsigned char Read(unsigned short address);
    virtual void Write(unsigned short address, unsigned char value);
    void SetValue(unsigned char value);
 private:
    int i = 0;
    // 4 msb = directional (down, up, left, right)
    // 4 lsb = buttons (start, select, b, a)
    unsigned char inputMap_ = 0;
    // represents bits 4 and 5, the input selection bits
    unsigned char joypad_select_ = 0b00000011;
    MemoryMapper* mapper_;
};
} // namespace memory
} // namespace back_end

#endif