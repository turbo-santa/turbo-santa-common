#ifndef TURBO_SANTA_COMMON_BACK_END_MEMORY_MEMORY_MAPPER_H_
#define TURBO_SANTA_COMMON_BACK_END_MEMORY_MEMORY_MAPPER_H_

#include "back_end/memory/memory_page.h"
#include "back_end/memory/memory_segment.h"

namespace back_end {
namespace memory {

class VideoRam;
class ObjectAttributeMemory;
class Unusable;
class IORegisters;
class ZeroPage;
class InteruptEnable;

class MemoryMapper {
    public:
        MemoryMapper(unsigned char* raw_rom, long rom_size);

        unsigned char* get_pointer() { return mapped_memory_.get(); }

        void set(unsigned short location, unsigned char value);


        unsigned char get(unsigned short location);

        static const long kROMSegmentSize = 0x4000;
        // TODO(Brendan): Put an actual size here.
        static const int kROMNumber = 1;
        static const long kRAMSegmentSize = 0x1000;
        // TODO(Brendan): Put an actual size here.
        static const int kRAMNumber = 1;

    private:
        std::unique_ptr<unsigned char> mapped_memory_;
        ReadOnlyMemorySegment rom_bank_0_;
        ReadOnlyMemorySegment rom_bank_1_;
        PageManager rom_bank_manager_;
        TransparentMemorySegment ram_bank_0_;
        TransparentMemorySegment ram_bank_1_;
        PageManager ram_bank_manager_;
//         VideoRam video_ram_;
//         TransparentMemorySegment cartridge_ram_;
//         ObjectAttributeMemory oam;
//         Unusable unusable_;
//         IORegisters i_o_registers_;
//         ZeroPage zero_page_;
//         InteruptEnable interupt_enable_;
        std::vector<WriteDispatcher> write_dispatchers = {
            CreateWriteDispatcher(0x0000, 0x3fff, &rom_bank_0_),
            CreateWriteDispatcher(0x4000, 0x7fff, &rom_bank_1_),
//             CreateWriteDispatcher(0x8000, 0x9fff, &video_ram_),
//             CreateWriteDispatcher(0xa000, 0xbfff, &cartridge_ram_),
            CreateWriteDispatcher(0xc000, 0xcfff, &ram_bank_0_),
            CreateWriteDispatcher(0xd000, 0xdfff, &ram_bank_1_),
            // Yeah, so the memory from 0xd000 to 0xdfff is the same memory
            // as 0xe000 to 0xfdff.
            CreateWriteDispatcher(0xe000, 0xfdff, &ram_bank_1_),
//             CreateWriteDispatcher(0xfe00, 0xfe9f, &oam_),
//             CreateWriteDispatcher(0xfea0, 0xfeff, &unusable_),
//             CreateWriteDispatcher(0xff00, 0xff7f, &i_o_registers_),
//             CreateWriteDispatcher(0xff80, 0xfffe, &zero_page_),
//             CreateWriteDispatcher(0xffff, &interupt_enable_)
        };

        std::vector<ReadDispatcher> read_dispatchers = {
            CreateReadDispatcher(0x0000, 0x3fff, &rom_bank_0_),
            CreateReadDispatcher(0x4000, 0x7fff, &rom_bank_1_),
//             CreateReadDispatcher(0x8000, 0x9fff, &video_ram_),
//             CreateReadDispatcher(0xa000, 0xbfff, &cartridge_ram_),
            CreateReadDispatcher(0xc000, 0xcfff, &ram_bank_0_),
            CreateReadDispatcher(0xd000, 0xdfff, &ram_bank_1_),
            // Yeah, so the memory from 0xd000 to 0xdfff is the same memory
            // as 0xe000 to 0xfdff.
            CreateReadDispatcher(0xe000, 0xfdff, &ram_bank_1_),
//             CreateReadDispatcher(0xfe00, 0xfe9f, &oam_),
//             CreateReadDispatcher(0xfea0, 0xfeff, &unusable_),
//             CreateReadDispatcher(0xff00, 0xff7f, &i_o_registers_),
//             CreateReadDispatcher(0xff80, 0xfffe, zero_page_),
//             CreateReadDispatcher(0xffff, interupt_enable_)
        };

};
} // namespace memory_mapper
} // namespace back_end
#endif // TURBO_SANTA_COMMON_BACK_END_MEMORY_MEMORY_MAPPER_H_
