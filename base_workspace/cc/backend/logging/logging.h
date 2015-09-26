#ifndef TURBO_SANTA_COMMON_BACK_END_LOGGING_LOGGING_H_
#define TURBO_SANTA_COMMON_BACK_END_LOGGING_LOGGING_H_

#include <memory>

#include "cc/backend/logging/message_controller.h"
#include "protobuf/backend/logging/message.pb.h"

namespace backend {
namespace logging {

std::unique_ptr<Message> ReadAccess(uint16_t address, uint8_t value);

std::unique_ptr<Message> WriteAccess(uint16_t address,
                                     uint8_t old_value, 
                                     uint8_t new_value);

} // namespace logging
} // namespace backend

#define PUBLISH_READ(address, value) \
    PUBLISH(backend::logging::ReadAccess(address, value));

#define PUBLISH_WRITE(address, old_value, new_value) \
    PUBLISH(backend::logging::WriteAccess(address, old_value, new_value));

#endif // TURBO_SANTA_COMMON_BACK_END_LOGGING_LOGGING_H_
