#ifndef TURBO_SANTA_COMMON_BACK_END_DEBUGGER_CALL_STACK_H_
#define TURBO_SANTA_COMMON_BACK_END_DEBUGGER_CALL_STACK_H_

#include "backend/config.h"

#include <stack>

namespace back_end {
namespace debugger {

struct FunctionCall {
  long timestamp;
  unsigned short address;
};

class CallStack {
 public:
  void Push(FunctionCall function_call) { call_stack_.push(function_call); }

  FunctionCall Pop() {
    FunctionCall function_call = call_stack_.top();
    call_stack_.pop();
    return function_call;
  }

  bool PeekCheck(unsigned short address) { return call_stack_.top().address == address; }

 private:
  std::stack<FunctionCall> call_stack_;
};

} // namespace debugger
} // namespace back_end

#endif // TURBO_SANTA_COMMON_BACK_END_DEBUGGER_CALL_STACK_H_
