#ifndef TURBO_SANTA_COMMON_BACKEND_LOGGING_COMMUNICATION_LAYER_H_
#define TURBO_SANTA_COMMON_BACKEND_LOGGING_COMMUNICATION_LAYER_H_

#include <memory>

#include "cc/utility/option.h"
#include "cc/utility/stream.h"
#include "protobuf/backend/logging/message.pb.h"

namespace backend {
namespace logging {

class CommunicationLayer {
 public:
  void PutMessage(std::unique_ptr<const Message> message); 

  std::vector<uint8_t> TakeMessage();

  bool is_closed() { return in_stream_.is_closed(); }

  void Close() { in_stream_.Close(); }

 private:
  utility::Stream<const Message> in_stream_;
};

} // namespace logging
} // namespace backend

#endif // TURBO_SANTA_COMMON_BACKEND_LOGGING_COMMUNICATION_LAYER_H_
