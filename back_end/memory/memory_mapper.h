#ifndef TURBO_SANTA_COMMON_BACK_END_MEMORY_MEMORY_MAPPER_H_
#define TURBO_SANTA_COMMON_BACK_END_MEMORY_MEMORY_MAPPER_H_

#include "back_end/memory/rom_bank.h"

namespace back_end {
namespace memory {
class MemoryMapper {
    public:

        unsigned char* get_rom_pointer() {
            return rom_bank_0_.get_rom_pointer();
        }

        void set(unsigned short location, unsigned char value) {
            if (0 <= location && location <= 0x3fff) {
                rom_bank_0_.set(location, value);
            } else if (0x4000 <= location && location <= 0x7fff) {
                rom_bank_manager_.set(location, value);
            } else if (0x8000 <= location && location <= 0x9fff) {
            } else if (0xa000 <= location && location <= 0xbfff) {
            } else if (0xc000 <= location && location <= 0xcfff) {
                ram_bank_0_.set(location, value);
            } else if (0xd000 <= location && location <= 0xdfff) {
                ram_bank_manager_.set(location, value);
            } else if (0xe000 <= location && location <= 0xfdff) {
                // Yeah, so the memory from 0xd000 to 0xdfff is the same memory
                // as 0xe000 to 0xfdff.
                ram_bank_manager_.set(location, value);
            } else if (0xfe00 <= location && location <= 0xfe9f) {
            } else if (0xfea0 <= location && location <= 0xfeff) {
            } else if (0xff00 <= location && location <= 0xff7f) {
            } else if (0xff80 <= location && location <= 0xfffe) {
            } else if (0xffff == location) {
            }
        }

        unsigned char get(unsigned short location) {
            if (0 <= location && location <= 0x3fff) {
                return rom_bank_0_.get(location);
            } else if (0x4000 <= location && location <= 0x7fff) {
                return rom_bank_manager_.get(location);
            } else if (0x8000 <= location && location <= 0x9fff) {
            } else if (0xa000 <= location && location <= 0xbfff) {
            } else if (0xc000 <= location && location <= 0xcfff) {
                return ram_bank_0_.get(location);
            } else if (0xd000 <= location && location <= 0xdfff) {
                return ram_bank_manager_.get(location);
            } else if (0xe000 <= location && location <= 0xfdff) {
                // Yeah, so the memory from 0xd000 to 0xdfff is the same memory
                // as 0xe000 to 0xfdff.
                return ram_bank_manager_.get(location);
            } else if (0xfe00 <= location && location <= 0xfe9f) {
            } else if (0xfea0 <= location && location <= 0xfeff) {
            } else if (0xff00 <= location && location <= 0xff7f) {
            } else if (0xff80 <= location && location <= 0xfffe) {
            } else if (0xffff == location) {
            }
            return 0;
        }

    private:
        ROMBankManager rom_bank_manager_;
        ROMBank0 rom_bank_0_;
        RAMBankManager ram_bank_manager_;
        RAMBank0 ram_bank_0_;
};
} // namespace memory_mapper
} // namespace back_end
#endif // TURBO_SANTA_COMMON_BACK_END_MEMORY_MEMORY_MAPPER_H_
