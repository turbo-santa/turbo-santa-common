#include "back_end/memory/memory_segment.h"

namespace back_end {
namespace memory {

Predicate CreateIsInRange(unsigned short min, unsigned short max) {
    return [min, max](unsigned short address) { return min <= address && address <= max; };
}

ReadAction CreateReadAction(MemorySegment* memory_segment) {
    return [memory_segment](unsigned short address) { return memory_segment->get(address); };
}

WriteAction CreateWriteAction(MemorySegment* memory_segment) {
    return [memory_segment](unsigned short address, unsigned char value) { return memory_segment->set(address, value); };
}

ReadDispatcher CreateReadDispatcher(unsigned short min, unsigned short max, MemorySegment* memory_segment) {
    return { CreateIsInRange(min, max), CreateReadAction(memory_segment) };
}

WriteDispatcher CreateWriteDispatcher(unsigned short min, unsigned short max, MemorySegment* memory_segment) {
    return { CreateIsInRange(min, max), CreateWriteAction(memory_segment) };
}

void ReadOnlyMemorySegment::set(unsigned short address, unsigned char value) {
    for (WriteDispatcher dispatcher : dispatchers_) {
        if (dispatcher.predicate(address)) {
            dispatcher.action(address, value);
        }
    }
}

} // namespace memory
} // namespace back_end
