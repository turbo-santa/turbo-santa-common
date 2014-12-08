#ifndef TURBO_SANTA_COMMON_BACK_END_MEMORY_JOYPAD_MEMORY_H_
#define TURBO_SANTA_COMMON_BACK_END_MEMORY_JOYPAD_MEMORY_H_

namespace back_end {
namespace memory {
class JoypadMemory : public SingleAddressSegment {
 public:
    JoypadMemory() : SingleAddressSegment(0xff00) {}
    
    virtual unsigned char Read(unsigned short address) {
      unsigned char value = inputMap;
      
      if (value >> 4 == 0) {
        // Directional Keys selected
        return (value & 0xf0) | ((inputMap & 0xf0) >> 4);
      } else if (value >> 5 == 0) {
        // Buttons selected
        return (value & 0xf0) | (inputMap & 0x0f);
      } else {
        LOG(FATAL) << "Tried to read joypad input with neither directional or buttons selected";
      }
    }

    virtual void Write(unsigned short address, unsigned char value) {
      // the user can only write to bits 4 and 5 0b00110000 = 0x30
      inputMap = (value & 0x30) | inputMap;
    }
 private:
    // 4 msb = directional (down, up, left, right)
    // 4 lsb = buttons (start, select, b, a)
    unsigned char inputMap = 0;
};
} // namespace memory
} // namespace back_end

#endif