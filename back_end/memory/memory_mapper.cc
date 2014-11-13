#include "back_end/memory/memory_mapper.h"

#include <glog/logging.h>

namespace back_end {
namespace memory {

unsigned char MemoryMapper::Read(unsigned short address) {
  if (rom_bank_0_->InRange(address)) {
    return rom_bank_0_->Read(address);
  } else if (rom_bank_n_->InRange(address)) {
    return rom_bank_n_->Read(address);
  } else if (video_ram_->InRange(address)) {
    return video_ram_->Read(address);
  } else if (cartridge_ram_->InRange(address)) {
    return cartridge_ram_->Read(address);
  } else if (internal_ram_->InRange(address)) {
    return internal_ram_->Read(address);
  } else if (echo_ram_->InRange(address)) {
    return echo_ram_->Read(address);
  } else if (sprite_attribute_table_->InRange(address)) {
    return sprite_attribute_table_->Read(address);
  } else if (not_usable_->InRange(address)) {
    LOG(ERROR) << "Attempted read in non-usable region, address = " << address;
    return not_usable_->Read(address);
  } else if (io_ports_->InRange(address)) {
    return io_ports_->Read(address);
  } else if (high_ram_->InRange(address)) {
    return high_ram_->Read(address);
  } else if (interupt_enable_register_->InRange(address)) {
    return interupt_enable_register_->Read(address);
  } else {
    LOG(FATAL) << "Read was attempted at non-existent memory address: " << address;
  }
}

void MemoryMapper::Write(unsigned short address, unsigned char value) {
  if (rom_bank_0_->InRange(address)) {
    rom_bank_0_->Write(address, value);
  } else if (rom_bank_n_->InRange(address)) {
    rom_bank_n_->Write(address, value);
  } else if (video_ram_->InRange(address)) {
    video_ram_->Write(address, value);
  } else if (cartridge_ram_->InRange(address)) {
    cartridge_ram_->Write(address, value);
  } else if (internal_ram_->InRange(address)) {
    internal_ram_->Write(address, value);
  } else if (echo_ram_->InRange(address)) {
    echo_ram_->Write(address, value);
  } else if (sprite_attribute_table_->InRange(address)) {
    sprite_attribute_table_->Write(address, value);
  } else if (not_usable_->InRange(address)) {
    LOG(ERROR) << "Attempted write in non-usable region, address = " << address 
        << " value = " << value;
    not_usable_->Write(address, value);
  } else if (io_ports_->InRange(address)) {
    io_ports_->Write(address, value);
  } else if (high_ram_->InRange(address)) {
    high_ram_->Write(address, value);
  } else if (interupt_enable_register_->InRange(address)) {
    interupt_enable_register_->Write(address, value);
  } else {
    LOG(FATAL) << "Write was attempted at non-existent memory address: " << address;
  }
}

} // namespace memory
} // namespace back_end
