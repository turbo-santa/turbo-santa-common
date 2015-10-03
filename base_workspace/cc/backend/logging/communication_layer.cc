#include "cc/backend/logging/communication_layer.h"

namespace backend {
namespace logging {

using std::unique_ptr;
using std::vector;
using utility::Option;

void CommunicationLayer::PutMessage(unique_ptr<const Message> message) { 
  in_stream_.Put(std::move(message));
}

vector<uint8_t> CommunicationLayer::TakeMessage() {
  Option<unique_ptr<const Message>> message = in_stream_.Take();
  MessageOption option;
  if (message.is_present()) {
    option.mutable_payload()->CopyFrom(*message.get());
  }
  vector<uint8_t> buffer(option.ByteSize());
  option.SerializeToArray(buffer.data(), buffer.size());
  return buffer;
}

} // namespace logging
} // namespace backend
