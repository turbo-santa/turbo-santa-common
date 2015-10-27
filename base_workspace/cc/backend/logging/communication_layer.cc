#include "cc/backend/logging/communication_layer.h"

namespace backend {
namespace logging {

using std::unique_ptr;
using std::vector;
using utility::Option;

void CommunicationLayer::PutMessage(unique_ptr<const Message> message) { 
  in_stream_.Put(&message);
}

vector<uint8_t> CommunicationLayer::TakeMessage() {
  MessageOption option;
  unique_ptr<const Message> message;
  if (in_stream_.Take(&message)) {
    option.set_allocated_payload(const_cast<Message*>(message.release()));
  }
  vector<uint8_t> buffer(option.ByteSize());
  option.SerializeToArray(buffer.data(), buffer.size());
  return buffer;
}

} // namespace logging
} // namespace backend
