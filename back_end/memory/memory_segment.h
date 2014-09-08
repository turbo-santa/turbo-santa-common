#ifndef TURBO_SANTA_COMMON_BACK_END_MEMORY_MEMORY_SEGMENT_H_
#define TURBO_SANTA_COMMON_BACK_END_MEMORY_MEMORY_SEGMENT_H_

#include <functional>
#include <vector>

namespace back_end {
namespace memory {

class MemorySegment {
    public:
        virtual unsigned char get(unsigned short address) = 0;

        virtual void set(unsigned short address, unsigned char value) = 0;
};

class TransparentMemorySegment : public MemorySegment {
    public:
        TransparentMemorySegment(unsigned char* raw_memory) : raw_memory_(raw_memory) {}

        virtual unsigned char get(unsigned short address) {
            return raw_memory_[address];
        }

        virtual void set(unsigned short address, unsigned char value) {
            raw_memory_[address] = value;
        }

    private:
        unsigned char* raw_memory_;
};

typedef std::function<bool(unsigned short address)> Predicate;
typedef std::function<void(unsigned short address, unsigned char value)> WriteAction;
typedef std::function<unsigned char (unsigned short address)> ReadAction;

struct ReadDispatcher {
    Predicate predicate;
    ReadAction action;
};

struct WriteDispatcher {
    Predicate predicate;
    WriteAction action;
};

Predicate CreateIsInRange(unsigned short min, unsigned short max);

ReadAction CreateReadAction(MemorySegment* memory_segment);

WriteAction CreateWriteAction(MemorySegment* memory_segment);

ReadDispatcher CreateReadDispatcher(unsigned short min, unsigned short max, MemorySegment* memory_segment);

WriteDispatcher CreateWriteDispatcher(unsigned short min, unsigned short max, MemorySegment* memory_segment);

class ReadOnlyMemorySegment : public TransparentMemorySegment {
    public:
        ReadOnlyMemorySegment(unsigned char* raw_memory) : TransparentMemorySegment(raw_memory) {}

        virtual void set(unsigned short address, unsigned char value);

    protected:
        std::vector<WriteDispatcher> dispatchers_;
};

} // namespace memory
} // namespace back_end
#endif // TURBO_SANTA_COMMON_BACK_END_MEMORY_MEMORY_SEGMENT_H_
