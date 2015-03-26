#include "backend/memory/joypad_memory.h"
#include "backend/memory/memory_mapper.h"

namespace back_end {
namespace memory {

JoypadMemory::JoypadMemory(MemoryMapper* mapper) : SingleAddressSegment(0xff00), mapper_(mapper) {}

unsigned char JoypadMemory::Read(unsigned short address) {
  if ((joypad_select_ & 1) == 0) {
    // Directional Keys selected
    LOG(WARNING) << "Reading JoypadInput - Directional Keys selected";
    return (joypad_select_ << 4) | ((inputMap_ & 0xf0) >> 4);
  } else if ((joypad_select_ >> 1) == 0) {
    // Buttons selected
    LOG(WARNING) << "Reading JoypadInput - Buttons selected";
    return ReadButtons();
  } else {
    LOG(WARNING) << "Tried to read joypad input with neither directional or buttons selected";
    return ReadButtons();
  }
}

unsigned char JoypadMemory::ReadButtons() {
  return (joypad_select_ << 4) | (inputMap_ & 0x0f);
}

void JoypadMemory::Write(unsigned short address, unsigned char value) {
  // the user can only write to bits 4 and 5 0b00110000
  LOG(WARNING) << "Writing JoypadInput - " << std::hex << 0x00 + value;
  joypad_select_ = (value & 0b00110000) >> 4;
}

void JoypadMemory::SetValue(unsigned char value) {
  LOG(WARNING) << "Setting Input - " << std::hex << 0x00 + value;
  inputMap_ = value;
  mapper_->interrupt_flag()->set_joypad(true);
} 

} // namespace memory
} // namespace back_end
