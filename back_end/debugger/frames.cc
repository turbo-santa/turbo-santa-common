#include "back_end/debugger/frames.h"

#include <utility>
#include "back_end/debugger/great_library.h"

namespace back_end {
namespace debugger {

void FrameFactory::SubmitFrame() {
  current_frame_.set_register_deltas(register_producer_->RetrieveDelta());
  current_frame_.set_memory_deltas(memory_producer_->RetrieveDelta());
  current_frame_.set_pc_delta(pc_producer_->RetrieveDelta());
  great_library_->SubmitFrame(std::move(current_frame_));
}

} // namespace debugger
} // namespace back_end
