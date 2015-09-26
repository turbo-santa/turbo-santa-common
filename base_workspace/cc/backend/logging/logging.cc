#include "cc/backend/logging/logging.h"

namespace backend {
namespace logging {

using std::unique_ptr;

namespace {
unique_ptr<Message> MakeMemoryAccess(MemoryAccess::AccessType access_type, 
                                     uint16_t address, 
                                     uint8_t old_value, 
                                     uint8_t new_value) {
  unique_ptr<Message> message = unique_ptr<Message>(new Message());
  MemoryAccess* memory_access = message->
      mutable_memory_message()->mutable_memory_access();

  memory_access->set_access_type(access_type);
  memory_access->set_address(address);
  memory_access->set_old_value(old_value);
  memory_access->set_new_value(new_value);
  return std::move(message);
}
} // namespace

unique_ptr<Message> ReadAccess(uint16_t address, uint8_t value) {
  return MakeMemoryAccess(MemoryAccess::READ, address, value, value);
}

unique_ptr<Message> WriteAccess(uint16_t address,
                                uint8_t old_value, 
                                uint8_t new_value) {
  return MakeMemoryAccess(MemoryAccess::WRITE, address, old_value, new_value);
}

} // namespace logging
} // namespace backend

