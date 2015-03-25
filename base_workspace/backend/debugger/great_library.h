#ifndef TURBO_SANTA_COMMON_BACK_END_DEBUGGER_GREAT_LIBRARY_H_
#define TURBO_SANTA_COMMON_BACK_END_DEBUGGER_GREAT_LIBRARY_H_

#include "back_end/config.h"

#include <list>
#include <string>
#include <glog/logging.h>
#include "back_end/debugger/frames.h"
#include "back_end/opcode_executor/opcode_executor.h"

namespace back_end {
namespace debugger {

class GreatLibrary {
 public:
  void SubmitFrame(Frame frame) {
    history_frame_list_.push_back(frame);
  }
  const Frame& first_frame() { return history_frame_list_.front(); }
  const Frame& last_frame() { return history_frame_list_.back(); }
  const Frame& frame(long timestamp) { 
    for (const Frame& frame : history_frame_list_) {
      if (frame.timestamp() == timestamp) {
        return frame;
      }
    }
    LOG(FATAL) << "Timestamp: " << timestamp << " does not exist.";
  }
  ConstFrameIterator begin() const { return history_frame_list_.begin(); }
  ConstFrameIterator end() const { return history_frame_list_.end(); }

 private:
  std::list<Frame> history_frame_list_;
};

} // namespace debugger
} // namespace back_end

#endif // TURBO_SANTA_COMMON_BACK_END_DEBUGGER_GREAT_LIBRARY_H_
