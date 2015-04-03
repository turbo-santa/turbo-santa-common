#include "backend/memory/mbc.h"

#include <stdio.h>
#include "submodules/glog/src/glog/logging.h"

namespace back_end {
namespace memory {

using std::unique_ptr;
using std::vector;

// TODO(Brendan): Finish this.
MBC* CreateNoMBC(unsigned char* program_rom, long size) {
  ROMBank rom_bank_0;
  ROMBank rom_bank_1;
  RAMBank ram_bank_0;
  CreateROMBanks(program_rom, size, &rom_bank_0, &rom_bank_1);
  return new NoMBC(rom_bank_0, rom_bank_1, ram_bank_0);
}

MBC* CreateMBC1(unsigned char* program_rom, long size) {
  ROMBank rom_bank_0;
  vector<ROMBank> rom_bank_n;
  vector<RAMBank> ram_bank_n;
  CreateROMBanks(program_rom, size, &rom_bank_0, &rom_bank_n);
  CreateRAMBanks(4, &ram_bank_n);
  return new MBC1(rom_bank_0, rom_bank_n, ram_bank_n);
}

void CreateROMBanks(unsigned char* rom, long rom_size, ROMBank* rom_bank_0, ROMBank* rom_bank_1) {
  int address = 0;
  int i = 0;
  while (i < MBC::kROMBank0Size && address < rom_size) {
    rom_bank_0->memory_[i] = rom[address];
    address++;
    i++;
  }

  i = 0;
  while (i < MBC::kROMBankNSize && address < rom_size) {
    rom_bank_1->memory_[i] = rom[address];
    address++;
    i++;
  }
}

void CreateROMBanks(unsigned char* rom, long rom_size, ROMBank* rom_bank_0, std::vector<ROMBank>* rom_bank_n) {
  LOG(INFO) << "Creating ROM banks";
  LOG(INFO) << "ROM size = " << rom_size;

  int address = 0;
  int i = 0;
  while (i < MBC::kROMBank0Size && address < rom_size) {
    rom_bank_0->memory_[i] = rom[address];
    address++;
    i++;
  }
  LOG(INFO) << "Created ROM 0";

  int n = 1;
  while (address < rom_size) {
    ROMBank rom_bank;
    i = 0;
    while (i < MBC::kROMBankNSize && address < rom_size) {
      rom_bank.memory_[i] = rom[address];
      address++;
      i++;
    }
    rom_bank_n->push_back(rom_bank);
    LOG(INFO) << "Created ROM " <<  n;
    n++;
  }
}

unsigned char ROMBank::Read(unsigned short address) {
  return memory_[address];
}

void ROMBank::ForceWrite(unsigned short address, unsigned char value) {
  memory_[address] = value;
}

void CreateRAMBanks(int bank_number, vector<RAMBank>* ram_bank_n) {
  for (int i = 0; i < bank_number; i++) {
    ram_bank_n->push_back(RAMBank());
  }
}

unsigned char RAMBank::Read(unsigned short address) {
  return memory_[address];
}

void RAMBank::Write(unsigned short address, unsigned char value) {
  memory_[address] = value;
}

unique_ptr<MBC> ConstructMBC(unsigned char* program_rom, long size) {
  MBC::CartridgeType cartridge_type = GetCartridgeType(program_rom[0x147]);
  // TODO(Brendan): We should have some type of check on the ROM/RAM size, I do
  // not know what the behavior should be if the ROM states an incorrect size.
//   int rom_bank_number = GetROMBankNumber(program_rom[0x148]);
//   int ram_bank_number = GetRAMBankNumber(program_rom[0x149]);

  switch (cartridge_type) {
    case MBC::ROM_ONLY:
    case MBC::ROM_AND_RAM:
    case MBC::ROM_AND_RAM_BATTERY:
      LOG(INFO) << "Creating NoMBC";
      return unique_ptr<MBC>(CreateNoMBC(program_rom, size));
    case MBC::MBC1:
    case MBC::MBC1_WITH_RAM:
    case MBC::MBC1_WITH_RAM_BATTERY:
      LOG(INFO) << "Creating MBC1";
      return unique_ptr<MBC>(CreateMBC1(program_rom, size));
    case MBC::UNSUPPORTED:
    default:
      LOG(FATAL) << "Cartridge Type, " << cartridge_type << ", is unsupported";
  }
}

MBC::CartridgeType GetCartridgeType(unsigned char value) {
  return static_cast<MBC::CartridgeType>(value);
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
    LOG(WARNING) << "Write attempted in ROM_0, address: " << std::hex << address << " value: " << std::hex << 0x0000 + value;
  } else if (0x4000 <= address && address <= 0x7fff) {
    LOG(WARNING) << "Write attempted in ROM_1, address: " << std::hex << address << " value: " << std::hex << 0x0000 + value;
  } else if (0xa000 <= address && address <= 0xbfff) {
    ram_bank_0_.Write(address - 0xa000, value);
  } else {
    LOG(FATAL) << "Write attempted outside of MBC region: " << address;
  }
}

void NoMBC::ForceWrite(unsigned short address, unsigned char value) {
  if (0x0000 <= address && address <= 0x3fff) {
    rom_bank_0_.ForceWrite(address - 0x0000, value);
  } else if (0x4000 <= address && address <= 0x7fff) {
    rom_bank_1_.ForceWrite(address - 0x4000, value);
  } else if (0xa000 <= address && address <= 0xbfff) {
    ram_bank_0_.ForceWrite(address - 0xa000, value);
  } else {
    LOG(FATAL) << "ForceWrite attempted outside of MBC region: " << address;
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
  } else if (0x4000 <= address && address <= 0x7fff) {
    return rom_bank_n_.Read(address - 0x4000);
  } else if (0xa000 <= address && address <= 0xbfff) {
    return ram_bank_n_.Read(address - 0xa000);
  }
  LOG(FATAL) << "Read attempted outside of MBC region: " << std::hex << address;
}

void MBC1::Write(unsigned short address, unsigned char value) {
  if (0x0000 <= address && address <= 0x1fff) {
    SetRAMEnabled(value);
    LOG(INFO) << std::hex << 0x0000 + value << " was written to RAM enable region";
    LOG(INFO) << "RAM enabled = " << ram_enabled_;
  } else if (0x2000 <= address && address <= 0x3fff) {
    bank_mode_register_.SetLowerBits(value);
    LOG(INFO) << "ROM bank " << static_cast<int>(bank_mode_register_.GetROMBank()) << " was selected";
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
      LOG(ERROR) << "Write was attempted when the RAM was disabled";
    }
  } else {
    LOG(FATAL) << "Write attempted outside of MBC region: " << address;
  }
}

void MBC1::ForceWrite(unsigned short address, unsigned char value) {
  if (0x0000 <= address && address <= 0x3fff) {
    rom_bank_0_.ForceWrite(address - 0x0000, value);
  } else if (0x4000 <= address && address <= 0x4000) {
    rom_bank_n_.ForceWrite(address - 0x4000, value);
  } else if (0xa000 <= address && address <= 0xbfff) {
    rom_bank_n_.ForceWrite(address - 0xa000, value);
  } else {
    LOG(FATAL) << "Read attempted outside of MBC region: " << address;
  }
}

void MBC1::SetRAMEnabled(unsigned char value) {
  // Any value with 0x0a in the lower 4 bits enables RAM and any other value
  // disables it.
  ram_enabled_ = (0x0a == (value & 0b00001111));
}

} // namespace memory
} // namespace back_end
