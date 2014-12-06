#ifndef TURBO_SANTA_COMMON_BACK_END_DEBUGGER_FRAMES_H_
#define TURBO_SANTA_COMMON_BACK_END_DEBUGGER_FRAMES_H_

#include "back_end/config.h"

#include <list>
#include <string>
#include <utility>
#include <vector>
#include "back_end/debugger/deltas.h"

namespace back_end {
namespace debugger {
class GreatLibrary;
} // namespace debugger
} // namespace back_end

namespace back_end {
namespace debugger {

class Frame {
 public:
  Frame() {}
  Frame(const Frame& frame) :
      register_deltas_(frame.register_deltas_),
      memory_deltas_(frame.memory_deltas_),
      pc_delta_(frame.pc_delta_),
      event_(frame.event_),
      str_instruction_(frame.str_instruction_),
      raw_instruction_(frame.raw_instruction_),
      raw_parameters_(frame.raw_parameters_),
      timestamp_(frame.timestamp_) {}
  Frame(Frame&& frame) : 
      register_deltas_(std::move(frame.register_deltas_)),
      memory_deltas_(std::move(frame.memory_deltas_)),
      pc_delta_(frame.pc_delta_),
      event_(frame.event_),
      str_instruction_(frame.str_instruction_),
      raw_instruction_(frame.raw_instruction_),
      raw_parameters_(frame.raw_parameters_),
      timestamp_(frame.timestamp_) {}

  const std::vector<RegisterDelta>& register_deltas() const { return register_deltas_; }
  const std::vector<MemoryDelta>& memory_deltas() const { return memory_deltas_; }
  const PCDelta& pc_delta() const { return pc_delta_; }
  const std::string& event() const { return event_; }
  const std::string& str_instruction() const { return str_instruction_; }
  unsigned short raw_instruction() const { return raw_instruction_; }
  unsigned short raw_parameters() const { return raw_parameters_; }
  long timestamp() const { return timestamp_; }

  void set_register_deltas(std::vector<RegisterDelta>&& register_deltas) { register_deltas_ = register_deltas; }
  void set_memory_deltas(std::vector<MemoryDelta>&& memory_deltas) { memory_deltas_ = memory_deltas; }
  void set_pc_delta(PCDelta pc_delta) { pc_delta_ = pc_delta; }
  void set_event(std::string event) { event_ = event; }
  void set_str_instruction(std::string str_instruction) { str_instruction_ = str_instruction; }
  void set_raw_instruction(unsigned short raw_instruction) { raw_instruction_ = raw_instruction; }
  void set_raw_parameters(unsigned short raw_parameters) { raw_parameters_ = raw_parameters; }
  void set_timestamp(long timestamp) { timestamp_ = timestamp; }

 private:
  std::vector<RegisterDelta> register_deltas_;
  std::vector<MemoryDelta> memory_deltas_;
  PCDelta pc_delta_;
  std::string event_;
  std::string str_instruction_;
  unsigned short raw_instruction_;
  unsigned short raw_parameters_;
  long timestamp_;
};

typedef std::list<Frame>::const_iterator ConstFrameIterator;

class FrameFactory {
 public:
  void SubmitFrame();
  void SetEvent(std::string event) { current_frame_.set_event(event); }
  void SetInstructionName(std::string instruction) { current_frame_.set_str_instruction(instruction); }
  void SetRawInstruction(unsigned short instruction) { current_frame_.set_raw_instruction(instruction); }
  void setRawParameters(unsigned short instruction) { current_frame_.set_raw_parameters(instruction); }
 private:
  GreatLibrary* great_library_;
  RegisterProducer* register_producer_;
  MemoryProducer* memory_producer_;
  PCProducer* pc_producer_;
  Frame current_frame_;
  long current_timestamp_ = 0;
};

} // namespace debugger
} // namespace back_end

#endif // TURBO_SANTA_COMMON_BACK_END_DEBUGGER_FRAMES_H_
