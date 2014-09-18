#ifndef TURBO_SANTA_COMMON_BACK_END_MEMORY_MEMORY_SEGMENT_H_
#define TURBO_SANTA_COMMON_BACK_END_MEMORY_MEMORY_SEGMENT_H_

#include <functional>
#include <vector>

namespace back_end {
namespace memory {

class MemorySegment {
    public:
        // Whether this is the memory segment that the address is in.
        virtual bool InRange(unsigned short address) {
            return lower_address_bound() <= address && address <= upper_address_bound();
        }

        // Read the value at this memory address.
        virtual unsigned char Read(unsigned short address) = 0;

        // Write this value to this memory address.
        virtual void Write(unsigned short address, unsigned char value) = 0;

    protected:
        // Inclusive lower bound of this memory segment.
        virtual unsigned short lower_address_bound() = 0;

        // Inclusive upper bound of this memory segment.
        virtual unsigned short upper_address_bound() = 0;
};

struct RawMemoryBlock {
    unsigned char* raw_memory_start;
    unsigned long size;
};

class ROMSegment : public MemorySegment {
    public:
        ROMSegment(
                RawMemoryBlock* memory_block,
                unsigned short lower_address_bound,
                unsigned short upper_address_bound) 
            : lower_address_bound_(lower_address_bound),
            upper_address_bound_(upper_address_bound),
            memory_block_(memory_block) {}

        virtual unsigned char Read(unsigned short address) {
            return memory_block_->raw_memory_start[translate_address(address)];
        }

        virtual void Write(unsigned short address, unsigned char value) {
            WriteHandler(address, value);
        }


    protected:
        virtual void WriteHandler(unsigned short address, unsigned char value) = 0;

        virtual unsigned short lower_address_bound() { return lower_address_bound_; }
        virtual unsigned short upper_address_bound() { return upper_address_bound_; }

    private:
        unsigned short translate_address(unsigned short address) { return address - lower_address_bound_; }
        
        unsigned short lower_address_bound_;
        unsigned short upper_address_bound_;
        RawMemoryBlock* memory_block_;
};

class ROMSegment0 : public ROMSegment {
    public:
        ROMSegment0(RawMemoryBlock* memory_block)
            : ROMSegment(memory_block, 0x0000, 0x3fff) {}

    protected:
        virtual void WriteHandler(unsigned short address, unsigned char value);
        virtual void RAMBankEnable(unsigned char value);
        virtual void ROMBankSelectLSByte(unsigned char value);
        virtual void ROMBankSelectMSByte(unsigned char value);
};

class ROMSegmentN : public ROMSegment {
    public:
        ROMSegmentN(RawMemoryBlock* memory_block)
            : ROMSegment(memory_block, 0x4000, 0x7fff) {}

    protected:
        virtual void WriteHandler(unsigned short address, unsigned char value);
        virtual void RAMBankSelect(unsigned char value);
        virtual void RAMROMSelect(unsigned char value);
};

} // namespace memory
} // namespace back_end
#endif // TURBO_SANTA_COMMON_BACK_END_MEMORY_MEMORY_SEGMENT_H_
