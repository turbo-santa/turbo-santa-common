#include "cc/backend/debug/memory_profiler/memory_access.h"

namespace backend {
namespace debug {
namespace memory_profiler {

using std::unique_ptr;

namespace {
unique_ptr<Message> MakeAccessMessage(MemoryAccess::Mode mode, 
                                      uint16_t address, 
                                      uint8_t old_value, 
                                      uint8_t new_value) {
  MemoryAccess* access = new MemoryAccess();
  access->set_mode(mode);
  access->set_address(address);
  access->set_old_value(old_value);
  access->set_new_value(new_value);
  return unique_ptr<Message>(static_cast<Message*>(access));
}
} // namspace

unique_ptr<Message> MakeReadMessage(uint16_t address, uint8_t value) {
  return MakeAccessMessage(MemoryAccess::READ, address, value, value);
}

unique_ptr<Message> MakeWriteMessage(uint16_t address, 
                                     uint8_t old_value, 
                                     uint8_t new_value) {
  return MakeAccessMessage(MemoryAccess::WRITE, address, old_value, new_value);
}


} // namespace memory_profiler
} // namespace debug
} // namespace backend

