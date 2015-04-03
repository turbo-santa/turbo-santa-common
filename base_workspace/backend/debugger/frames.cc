#include "backend/debugger/frames.h"

#include <utility>
#include "backend/debugger/great_library.h"
#include "submodules/glog/src/glog/logging.h"

namespace back_end {
namespace debugger {

void FrameFactory::SubmitFrame() {
  current_frame_.set_register_deltas(register_producer_->RetrieveDelta());
  current_frame_.set_memory_deltas(memory_producer_->RetrieveDelta());
  current_frame_.set_pc_delta(pc_producer_->RetrieveDelta());
  current_frame_.set_timestamp(current_timestamp_);
  LOG(INFO) << "Submitting frame";
  great_library_->SubmitFrame(current_frame_);
  current_timestamp_++;
}

} // namespace debugger
} // namespace back_end
