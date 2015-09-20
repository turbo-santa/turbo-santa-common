#ifndef TURBO_SANTA_COMMON_BACK_END_DEBUG_MESSAGE_H_
#define TURBO_SANTA_COMMON_BACK_END_DEBUG_MESSAGE_H_

namespace backend {
namespace debug {

class Message {
 public:
  enum BaseType {
    MEMORY,
    GRAPHICS,
    OPCODE_EXECUTOR,
  };

  virtual BaseType base_type() const = 0;
};

} // namespace debug
} // namespace backend

#endif // TURBO_SANTA_COMMON_BACK_END_DEBUG_MESSAGE_H_
