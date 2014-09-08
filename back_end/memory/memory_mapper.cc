#include "back_end/memory/memory_mapper.h"

namespace back_end {
namespace memory {

MemoryMapper::MemoryMapper(unsigned char* raw_rom, long rom_size) 
    : mapped_memory_(std::unique_ptr<unsigned char>(new unsigned char[0x10000])),
    rom_bank_0_(mapped_memory_.get()),
    rom_bank_1_(mapped_memory_.get() + kROMSegmentSize),
    rom_bank_manager_(mapped_memory_.get() + kROMSegmentSize, kROMSegmentSize, kROMNumber),
    ram_bank_0_(mapped_memory_.get() + 0xc000),
    ram_bank_1_(mapped_memory_.get() + 0xd000),
    ram_bank_manager_(mapped_memory_.get() + 0xd000, kRAMSegmentSize, kRAMNumber) {
        int i = 0;
        for (; i < rom_size; i++) {
            mapped_memory_.get()[i] = raw_rom[i];
            break;
        }
        for (; i < 0x10000; i++) {
            mapped_memory_.get()[i] = 0;
        }
    }

void MemoryMapper::set(unsigned short location, unsigned char value) {
    for (WriteDispatcher dispatcher : write_dispatchers) {
        if (dispatcher.predicate(location)) {
            dispatcher.action(location, value);
        }
    }
}

unsigned char MemoryMapper::get(unsigned short location) {
    for (ReadDispatcher dispatcher : read_dispatchers) {
        if (dispatcher.predicate(location)) {
            return dispatcher.action(location);
        }
    }
    return 0;
}

} // namespace memory
} // namespace back_end
