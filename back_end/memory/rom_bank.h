#ifndef TURBO_SANTA_COMMON_BACK_END_MEMORY_ROM_BANK_H_
#define TURBO_SANTA_COMMON_BACK_END_MEMORY_ROM_BANK_H_

#include <memory>
#include <vector>

namespace back_end {
namespace memory {

class MemoryBank {
    public:
        virtual unsigned char get(unsigned short address) = 0;

        virtual void set(unsigned short address, unsigned char value) = 0;

        static const int kBankSize = 4000;
};

class ROMBankN : public MemoryBank {
    public:
        ROMBankN(unsigned char* raw_memory) : raw_memory_(raw_memory) {}

        virtual unsigned char get(unsigned short address) {
            return raw_memory_.get()[address];
        }

        virtual void CopyTo(unsigned char* raw_memory) {
            for (int i = 0; i < kBankSize; i++) {
                raw_memory[i] = raw_memory_.get()[i];
            }
        }

        virtual void set(unsigned short, unsigned char) {
            // TODO(Brendan): Throw some type of error here. The MemoryMapper
            // should probably intercept this.
        }

    protected:
        std::unique_ptr<unsigned char> raw_memory_;
};

class RAMBankN : public ROMBankN {
    public:
        RAMBankN() : ROMBankN(new unsigned char[kBankSize]) {}

        virtual void set(unsigned short address, unsigned char value) {
            raw_memory_.get()[address] = value;
        }
};

class BankManager {
    public:
        virtual unsigned char get(unsigned short address) {
            return selected_bank_->get(address);
        }

        virtual void set(unsigned short address, unsigned char value) = 0;

        virtual void select_bank(int bank_number) {
            selected_bank_ = banks_[bank_number].get();
        }

    protected:
        ROMBankN* selected_bank_;
        std::vector<std::unique_ptr<ROMBankN>> banks_;
};

class ROMBankManager : public BankManager {
    public:
        ROMBankManager(unsigned char* raw_memory) {
            for (int i = 0; i < kNumberOfROMBanks; i++) {
                banks_.push_back(std::unique_ptr<ROMBankN>(new ROMBankN(raw_memory)));
                raw_memory += MemoryBank::kBankSize;
            }
        }

        virtual void set(unsigned short, unsigned char) {
            // TODO(Brendan): Throw some type of error here. The MemoryMapper
            // should probably intercept this.
        }

        // TODO(Brendan): This should be configurable.
        static const int kNumberOfROMBanks = 16;
};

class RAMBankManager : public BankManager {
    public:
        RAMBankManager() {
            for (int i = 0; i < kNumberOfRAMBanks; i++) {
                banks_.push_back(std::unique_ptr<ROMBankN>(new RAMBankN()));
            }
        }

        virtual void set(unsigned short address, unsigned char value) {
            static_cast<RAMBankN*>(selected_bank_)->set(address, value);
        }

        // TODO(Brendan): This should be configurable.
        static const int kNumberOfRAMBanks = 4;
};

class ROMBank0 : public MemoryBank {
    public:
        ROMBank0(unsigned char* raw_memory) {
            for (int i = 0; i < kBankSize; i++) {
                rom_[i] = raw_memory[i];
            }
        }

        virtual void RemapROMBankN(ROMBankN* rom_bank_n) {
            rom_bank_n->CopyTo(rom_ + kBankSize);
        }

        virtual unsigned char get(unsigned short address) {
            return rom_[address];
        }

        virtual void set(unsigned short address, unsigned char value) {
            rom_[address] = value;
        }

        virtual unsigned char* get_rom_pointer() {
            return rom_;
        }

    private:
        unsigned char rom_[kBankSize * 2];
};

} // namespace memory
} // namespace back_end
#endif // TURBO_SANTA_COMMON_BACK_END_MEMORY_ROM_BANK_H_
