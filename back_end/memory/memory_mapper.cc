#include "back_end/memory/memory_mapper.h"

#include <stdio.h>
#include <glog/logging.h>

namespace back_end {
namespace memory {

using std::unique_ptr;

MemoryMapper::MemoryMapper() {
  unsigned char data = 0x00;
  mbc_ = unique_ptr<MBC>(CreateNoMBC(&data, 1));
}

unsigned char MemoryMapper::Read(unsigned short address) {
  if (mbc_->InRange(address)) {
    return mbc_->Read(address);
  } else if (video_ram_->InRange(address)) {
    return video_ram_->Read(address);
  } else if (internal_ram_->InRange(address)) {
    return internal_ram_->Read(address);
  } else if (echo_ram_->InRange(address)) {
    return echo_ram_->Read(address);
  } else if (sprite_attribute_table_->InRange(address)) {
    return sprite_attribute_table_->Read(address);
  } else if (not_usable_->InRange(address)) {
    // LOG(ERROR) << "Attempted read in non-usable region, address = " << address;
    return 0;
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
  if (mbc_->InRange(address)) {
    mbc_->Write(address, value);
  } else if (video_ram_->InRange(address)) {
    video_ram_->Write(address, value);
  } else if (internal_ram_->InRange(address)) {
    internal_ram_->Write(address, value);
  } else if (echo_ram_->InRange(address)) {
    echo_ram_->Write(address, value);
  } else if (sprite_attribute_table_->InRange(address)) {
    sprite_attribute_table_->Write(address, value);
  } else if (not_usable_->InRange(address)) {
    // LOG(ERROR) << "Attempted write in non-usable region, address = " << address 
    //     << " value = " << value;
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
