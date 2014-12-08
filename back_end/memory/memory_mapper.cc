#include "back_end/memory/memory_mapper.h"

#include <stdio.h>
#include <glog/logging.h>

namespace back_end {
namespace memory {

using std::unique_ptr;

MemoryMapper::MemoryMapper() {
  internal_rom_flag_->set();

  unsigned char data = 0x00;
  mbc_ = unique_ptr<MBC>(CreateNoMBC(&data, 1));
}

MemoryMapper::MemoryMapper(unsigned char* rom, long size, bool use_internal_rom) {
  if (!use_internal_rom) {
    internal_rom_flag_->set();
  }

  mbc_ = ConstructMBC(rom, size);
}

void MemoryMapper::Enable() {
}

void MemoryMapper::Disable() {
}

unsigned char MemoryMapper::Read(unsigned short address) {
  if (address == 0xff80) {
    LOG(INFO) << "Reading from 0xff80, value = " << std::hex << 0x0000 + high_ram_->Read(address);
  }
  if (address == 0xff85) {
    LOG(INFO) << "Reading from 0xff85, value = " << std::hex << 0x0000 + high_ram_->Read(address);
  }

  if (!internal_rom_flag_->is_set() && internal_rom_->InRange(address)) {
    return internal_rom_->Read(address);
  }

  if (mbc_->InRange(address)) {
    return mbc_->Read(address);
  } else if (video_ram_->InRange(address)) {
    return video_ram_->Read(address);
  } else if (internal_ram_0_->InRange(address)) {
    return internal_ram_0_->Read(address);
  } else if (internal_ram_1_->InRange(address)) {
    return internal_ram_1_->Read(address);
  } else if (echo_ram_->InRange(address)) {
    return echo_ram_->Read(address);
  } else if (sprite_attribute_table_->InRange(address)) {
    return sprite_attribute_table_->Read(address);
  } else if (not_usable_->InRange(address)) {
    LOG(ERROR) << "Attempted read in non-usable region, address = " << std::hex << address;
    return 0;
    return not_usable_->Read(address);
  } else if (interrupt_flag_->InRange(address)) {
    return interrupt_flag_->Read(address);
  } else if (graphics_flags_->InRange(address)) {
    return graphics_flags_->Read(address);
  } else if (dma_transfer_->InRange(address)) {
    return dma_transfer_->Read(address);
  } else if (internal_rom_flag_->InRange(address)) {
    return internal_rom_flag_->Read(address);
  } else if (joypad_->InRange(address)) {
    return joypad_->Read(address);
  } else if (io_ports_->InRange(address)) {
    return io_ports_->Read(address);
  } else if (high_ram_->InRange(address)) {
    return high_ram_->Read(address);
  } else if (interrupt_enable_->InRange(address)) {
    return interrupt_enable_->Read(address);
  } else {
    LOG(FATAL) << "Read was attempted at non-existent memory address: " << std::hex << address;
  }
}

void MemoryMapper::Write(unsigned short address, unsigned char value) {
  if (address == 0xff80) {
    LOG(INFO) << "Writing to 0xff80, value = " << std::hex << 0x0000 + value;
  }
  if (address == 0xff85) {
    LOG(INFO) << "Writing to 0xff85, value = " << std::hex << 0x0000 + value;
  }

  memory_producer_.MemoryWrite(address, Read(address), value);

  if (mbc_->InRange(address)) {
    mbc_->Write(address, value);
  } else if (video_ram_->InRange(address)) {
    video_ram_->Write(address, value);
  } else if (internal_ram_0_->InRange(address)) {
    internal_ram_0_->Write(address, value);
  } else if (internal_ram_1_->InRange(address)) {
    internal_ram_1_->Write(address, value);
  } else if (echo_ram_->InRange(address)) {
    echo_ram_->Write(address, value);
  } else if (sprite_attribute_table_->InRange(address)) {
    sprite_attribute_table_->Write(address, value);
  } else if (not_usable_->InRange(address)) {
    LOG(ERROR) << "Attempted write in non-usable region, address: " << std::hex << address 
        << " value: " << std::hex << 0x0000 + value;
    not_usable_->Write(address, value);
  } else if (interrupt_flag_->InRange(address)) {
    interrupt_flag_->Write(address, value);
  } else if (graphics_flags_->InRange(address)) {
    graphics_flags_->Write(address, value);
  } else if (dma_transfer_->InRange(address)) {
    dma_transfer_->Write(address, value);
  } else if (internal_rom_flag_->InRange(address)) {
    internal_rom_flag_->Write(address, value);
  } else if (joypad_->InRange(address)) {
    joypad_->Write(address, value);
  } else if (io_ports_->InRange(address)) {
    io_ports_->Write(address, value);
  } else if (high_ram_->InRange(address)) {
    high_ram_->Write(address, value);
  } else if (interrupt_enable_->InRange(address)) {
    interrupt_enable_->Write(address, value);
  } else {
    LOG(FATAL) << "Write was attempted at non-existent memory address: " << std::hex << address;
  }
}

} // namespace memory
} // namespace back_end
