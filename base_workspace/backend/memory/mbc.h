#ifndef TURBO_SANTA_COMMON_BACK_END_MEMORY_MBC_H_
#define TURBO_SANTA_COMMON_BACK_END_MEMORY_MBC_H_

#include "backend/config.h"

#include <memory>
#include <vector>

#include "backend/memory/memory_segment.h"

namespace test_harness {
class TestHarness;
} // namespace test_harness

namespace back_end {
namespace clocktroller {
class ClocktrollerTest;
}
}

namespace back_end {
namespace memory {

class ROMBank;

void CreateROMBanks(unsigned char* rom, long rom_size, ROMBank* rom_bank_0, ROMBank* rom_bank_1);
void CreateROMBanks(unsigned char* rom, long rom_size, ROMBank* rom_bank_0, std::vector<ROMBank>* rom_bank_n);

class ROMBank {
 public:
  ROMBank() : memory_(0x4000, 0x00) {}

  virtual unsigned char Read(unsigned short address);

  virtual void ForceWrite(unsigned short address, unsigned char value);

 private:
  std::vector<unsigned char> memory_;

  friend void CreateROMBanks(unsigned char* rom, long rom_size, ROMBank* rom_bank_0, ROMBank* rom_bank_1);
  friend void CreateROMBanks(unsigned char* rom, long rom_size, ROMBank* rom_bank_0, std::vector<ROMBank>* rom_bank_1);
};

class RAMBank;

void CreateRAMBanks(int bank_number, std::vector<RAMBank>* ram_bank_n_);

class RAMBank {
 public:
  RAMBank() : memory_(0x2000, 0x00) {}

  virtual unsigned char Read(unsigned short address);

  virtual void Write(unsigned short address, unsigned char value);

  virtual void ForceWrite(unsigned short address, unsigned char value) {
    Write(address, value);
  }

 private:
  std::vector<unsigned char> memory_;
  friend void CreateRAMBanks(int bank_number, std::vector<RAMBank>* ram_bank_n_);
};

class MBC : public MemorySegment {
  public:
    static const unsigned short kROMBank0Size = 0x4000;
    static const unsigned short kROMBankNSize = 0x4000;
    static const unsigned short kRAMBankNSize = 0x2000;

    enum CartridgeType {
      ROM_ONLY = 0x00,
      MBC1 = 0x01,
      MBC1_WITH_RAM = 0x02,
      MBC1_WITH_RAM_BATTERY = 0x03, // We do not currently have support for battery.
      ROM_AND_RAM = 0x08,
      ROM_AND_RAM_BATTERY = 0x09, // We do not currently have support for battery.
      UNSUPPORTED = 0xdd // 0xdd is unused so this is safe.
    };

    virtual unsigned char Read(unsigned short address) = 0;
    virtual void Write(unsigned short address, unsigned char value) = 0;

    virtual bool InRange(unsigned short address) { 
      return (0x0000 <= address && address <= 0x7fff) || (0xa000 <= address && address <= 0xbfff);
    }
    
  protected:
    virtual unsigned short lower_address_bound() { return 0x0000; }
    virtual unsigned short upper_address_bound() { return 0xbfff; }
    virtual void ForceWrite(unsigned short address, unsigned char value) = 0;
    friend class test_harness::TestHarness;
    friend class clocktroller::ClocktrollerTest;
};

std::unique_ptr<MBC> ConstructMBC(unsigned char* program_rom, long size);

MBC* CreateNoMBC(unsigned char* program_rom, long size);

MBC* CreateMBC1(unsigned char* program_rom, long size);

MBC::CartridgeType GetCartridgeType(unsigned char cartridge_type_value);

int GetROMBankNumber(unsigned char rom_size_value);

int GetRAMBankNumber(unsigned char ram_size_value);

class NoMBC : public MBC {
 public:
  NoMBC(ROMBank rom_bank_0, ROMBank rom_bank_1, RAMBank ram_bank_0)
      : rom_bank_0_(rom_bank_0), rom_bank_1_(rom_bank_1), ram_bank_0_(ram_bank_0) {}

  virtual unsigned char Read(unsigned short address);
  virtual void Write(unsigned short address, unsigned char value);

 protected:
  ROMBank rom_bank_0_;
  ROMBank rom_bank_1_;
  RAMBank ram_bank_0_;

  virtual void ForceWrite(unsigned short address, unsigned char value);
};

class MBC1 : public MBC {
  public:
   MBC1(ROMBank rom_bank_0, std::vector<ROMBank> rom_bank_n, std::vector<RAMBank> ram_bank_n)
       : rom_bank_0_(rom_bank_0), rom_bank_n_(rom_bank_n, &bank_mode_register_), ram_bank_n_(ram_bank_n, &bank_mode_register_) {}

    virtual unsigned char Read(unsigned short address);
    virtual void Write(unsigned short address, unsigned char value);
   
    // The documentation stated
    // that the gameboy game may change the ROM/RAM addressing mode at anytime
    // and stated that RAM bank 0 may still be accessed in ROM mode while ROM
    // banks addressed with only with lower bits may be accessed in RAM mode.
    // It did not, however, specify what the behavior is when the RAM bank was
    // non zero when disabled. It would make the most sense that it is assumed
    // zero. Based on the documentation, it appears that all of this is controled
    // through a single 8-bit register with the 5 LSB specifying the 5 LSB of
    // the ROM bank, the next 2 bits setting the 2 MSB of the the ROM bank or
    // the RAM bank number depending on the mode. The mode is then controlled
    // by the MSB.
    class BankModeRegister {
      public:
        // Sets the bottom 5 bits of the BankModeRegister.
        void SetLowerBits(unsigned char value);

        // Sets bits 6 and 7 of the BankModeRegister.
        void SetUpperBits(unsigned char value);

        // Sets whether is ROM mode or RAM mode.
        void SetIsRAMMode(unsigned char value);

        // Gets the number of the selected RAM bank.
        unsigned char GetRAMBank();

        // Gets the number of the selected ROM bank.
        unsigned char GetROMBank();

      private:
        // 7-bit register that stores that sets the selected ROM/RAM address(es).
        unsigned char register_ = 0;
        bool is_ram_mode_ = false;
    };

    class ROMBankN {
      public:
       ROMBankN(std::vector<ROMBank> banks, BankModeRegister* bank_mode_register) : 
           banks_(banks), bank_mode_register_(bank_mode_register) {}

        virtual unsigned char Read(unsigned short address) {
          return banks_[ComputeROMBank()].Read(address);
        }

        virtual void ForceWrite(unsigned short address, unsigned char value) {
          banks_[ComputeROMBank()].ForceWrite(address, value);
        }

      private:
        // Unlike the RAM bank, the ROM bank number does not directly correspond
        // to that banks index in the vector of banks. When the user sets the
        // bottom 5 bits of the RAM/ROM bank register (the BankModeRegister here)
        // to 0 it actually gets set to 1 since bank 0 is always mapped into
        // memory at a different range of addresses. Thus we have to translate
        // the given ROM bank number to its index in the vector.
        unsigned char ComputeROMBank();

        std::vector<ROMBank> banks_;
        BankModeRegister* bank_mode_register_;
    };

    class RAMBankN {
      public:
       RAMBankN(std::vector<RAMBank> banks, BankModeRegister* bank_mode_register) : 
           banks_(banks), bank_mode_register_(bank_mode_register) {}

        virtual unsigned char Read(unsigned short address) {
          return banks_[bank_mode_register_->GetRAMBank()].Read(address);
        }

        virtual void Write(unsigned short address, unsigned char value) {
          banks_[bank_mode_register_->GetRAMBank()].Write(address, value);
        }

        virtual void ForceWrite(unsigned short address, unsigned char value) {
          Write(address, value);
        }

      private:
        std::vector<RAMBank> banks_;
        BankModeRegister* bank_mode_register_;
    };

  private:
    void SetRAMEnabled(unsigned char value);
    virtual void ForceWrite(unsigned short address, unsigned char value);
    
    bool ram_enabled_ = true;
    BankModeRegister bank_mode_register_;
    ROMBank rom_bank_0_;
    ROMBankN rom_bank_n_;
    RAMBankN ram_bank_n_;
};

} // namespace memory
} // namespace back_end
#endif // TURBO_SANTA_COMMON_BACK_END_MEMORY_MBC_H_
