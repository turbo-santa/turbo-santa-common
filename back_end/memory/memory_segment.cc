#include "back_end/memory/memory_segment.h"

namespace back_end {
namespace memory {

void ROMSegment0::WriteHandler(unsigned short address, unsigned char value) {
    if (0x0000 <= address && address <= 0x1fff) {
        RAMBankEnable(value);
    } else if (0x2000 <= address && address <= 0x2fff) {
        ROMBankSelectLSByte(value);
    } else if (0x3000 <= address && address <= 0x3fff) {
        ROMBankSelectMSByte(value);
    }
}

void ROMSegment0::RAMBankEnable(unsigned char value);

void ROMSegment0::ROMBankSelectLSByte(unsigned char value);

void ROMSegment0::ROMBankSelectMSByte(unsigned char value);

void ROMSegmentN::WriteHandler(unsigned short address, unsigned char value) {
    if (0x4000 <= address && address <= 0x5fff) {
        RAMBankSelect(value);
    } else if (0x6000 <= address && address <= 0x7fff) {
        RAMROMSelect(value);
    }
}

void ROMSegmentN::RAMBankSelect(unsigned char value);

void ROMSegmentN::RAMROMSelect(unsigned char value);

} // namespace memory
} // namespace back_end
