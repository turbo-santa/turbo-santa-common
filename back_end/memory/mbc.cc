#include "back_end/memory/mbc.h"

#include <glog/logging.h>

namespace back_end {
namespace memory {

unsigned char ROMBank::Read(unsigned short address) {
  return memory_->raw_memory_start[address];
}

unsigned char RAMBank::Read(unsigned short address) {
  return memory_->raw_memory_start[address];
}

void RAMBank::Write(unsigned short address, unsigned char value) {
  memory_->raw_memory_start[address] = value;
}

unsigned char NoMBC::Read(unsigned short address) {
  if (0x0000 <= address && address <= 0x3fff) {
    return rom_bank_0_.Read(address - 0x0000);
  } else if (0x4000 <= address && address <= 0x7fff) {
    return rom_bank_1_.Read(address - 0x4000);
  } else if (0xa000 <= address && address <= 0xbfff) {
    return ram_bank_0_.Read(address - 0xa000);
  }
  LOG(FATAL) << "Read attempted outside of MBC region: " << address;
}

void NoMBC::Write(unsigned short address, unsigned char value) {
  if (0x0000 <= address && address <= 0x3fff) {
    LOG(WARNING) << "Write attempted in ROM_0: " << value;
  } else if (0x4000 <= address && address <= 0x7fff) {
    LOG(WARNING) << "Write attempted in ROM_1: " << value;
  } else if (0xa000 <= address && address <= 0xbfff) {
    ram_bank_0_.Write(address - 0xa000, value);
  } else {
    LOG(FATAL) << "Write attempted outside of MBC region: " << address;
  }
}

void MBC1::BankModeRegister::SetLowerBits(unsigned char value) {
  if ((value & 0b00011111) == 0x00) {
    // The MBC translates writing 0x00 here to writing 0x01.
    register_ = 0b00000001;
  } else {
    register_ = (register_ & 0b11100000) | (value & 0b00011111);
  }
}

void MBC1::BankModeRegister::SetUpperBits(unsigned char value) {
  value = (value & 0b00000011) << 5;
  register_ = (register_ & 0b00011111) | value;
}

void MBC1::BankModeRegister::SetIsRAMMode(unsigned char value) {
  if (value & 0b00000001) {
    is_ram_mode_ = true;
  } else {
    is_ram_mode_ = false;
  }
}

unsigned char MBC1::BankModeRegister::GetRAMBank() {
  if (is_ram_mode_) {
    return (register_ & 0b01100000) >> 5;
  } else {
    return 0;
  }
}

unsigned char MBC1::BankModeRegister::GetROMBank() {
  if (is_ram_mode_) {
    return register_ & 0b00011111;
  } else {
    return register_ & 0b01111111;
  }
}

unsigned char MBC1::ROMBankN::ComputeROMBank() {
  unsigned char bank_number = bank_mode_register_->GetROMBank();
  
  // Since certain bank values are skipped we must translate bank address space
  // into the index space with respect to the vector which holds the actual
  // memory blocks.
  if (bank_number > 0x60) {
    return bank_number - 4;
  } else if (bank_number > 0x40) {
    return bank_number - 3;
  } else if (bank_number > 0x20) {
    return bank_number - 2;
  } else {
    return bank_number - 1;
  }
}

unsigned char MBC1::Read(unsigned short address) {
  if (0x0000 <= address && address <= 0x3fff) {
    return rom_bank_0_.Read(address - 0x0000);
  } else if (0x4000 <= address && address <= 0x4000) {
    return rom_bank_n_.Read(address - 0x4000);
  } else if (0xa000 <= address && address <= 0xbfff) {
    return rom_bank_n_.Read(address - 0xa000);
  }
  LOG(FATAL) << "Read attempted outside of MBC region: " << address;
}

void MBC1::Write(unsigned short address, unsigned char value) {
  if (0x0000 <= address && address <= 0x1fff) {
    SetRAMEnabled(value);
  } else if (0x2000 <= address && address <= 0x3fff) {
    bank_mode_register_.SetLowerBits(value);
  } else if (0x4000 <= address && address <= 0x5fff) {
    bank_mode_register_.SetUpperBits(value);
  } else if (0x6000 <= address && address <= 0x7fff) {
    bank_mode_register_.SetIsRAMMode(value);
  } else if (0xa000 <= address && address <= 0xbfff) {
    if (ram_enabled_) {
      ram_bank_n_.Write(address - 0xa000, value);
    } else {
      // TODO(Brendan): Determine correct behavior when possible.
      // The documentation was not entirely clear as to what to do when the RAM
      // is written to and is not enabled.
      LOG(ERROR) << "Write was attempted when either the RAM was disabled";
    }
  } else {
    LOG(FATAL) << "Write attempted outside of MBC region: " << address;
  }
}

void MBC1::SetRAMEnabled(unsigned char value) {
  // Any value with 0x0a in the lower 4 bits enables RAM and any other value
  // disables it.
  ram_enabled_ = 0x0a == (value & 0b00001111);
}

} // namespace memory
} // namespace back_end
