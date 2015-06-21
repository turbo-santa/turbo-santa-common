#include "backend/opcode_executor/opcode_executor.h"

#include "backend/decompiler/decompiler.h"
#include "backend/decompiler/decompiler_factory.h"
#include "backend/decompiler/instruction.h"
#include "backend/decompiler/rom_reader.h"
#include "backend/opcode_executor/opcode_handlers.h"
#include "submodules/glog/src/glog/logging.h"

namespace back_end {
namespace opcode_executor {

using opcodes::Opcode;
using decompiler::ArgumentType;
using decompiler::DecompilerFactory;
using decompiler::Instruction;
using decompiler::Parameter;
using decompiler::Register;
using decompiler::ROMReader;
using registers::GB_CPU;

namespace {
uint16_t GetRegisterValue16Bit(Register reg, const GB_CPU& cpu) {
  switch (reg) {
    case Register::A:
      return cpu.flag_struct.rA;
    case Register::B:
      return cpu.bc_struct.rB;
    case Register::C:
      return cpu.bc_struct.rC;
    case Register::D:
      return cpu.de_struct.rD;
    case Register::E:
      return cpu.de_struct.rE;
    case Register::H:
      return cpu.hl_struct.rH;
    case Register::L:
      return cpu.hl_struct.rL;
    case Register::AF:
      return cpu.rAF;
    case Register::BC:
      return cpu.rBC;
    case Register::DE:
      return cpu.rDE;
    case Register::HL:
      return cpu.rHL;
    case Register::SP:
      return cpu.rSP;
    case Register::PC:
      return cpu.rPC;
    case Register::ZF:
      return cpu.flag_struct.rF.Z;
    case Register::NF:
      return cpu.flag_struct.rF.N;
    case Register::HF:
      return cpu.flag_struct.rF.H;
    case Register::CF:
      return cpu.flag_struct.rF.C;
    case Register::ZN:
      return cpu.flag_struct.rF.Z;
    case Register::NN:
      return cpu.flag_struct.rF.N;
    case Register::HN:
      return cpu.flag_struct.rF.H;
    case Register::CN:
      return cpu.flag_struct.rF.C;
  }
}

uint16_t GetParameterValue16Bit(const Parameter& param, const GB_CPU& cpu) {
  switch (param.type) {
    case ArgumentType::REGISTER:
      return GetRegisterValue16Bit(param.value.reg, cpu);
    case ArgumentType::VALUE:
      return param.value.val;
    case ArgumentType::EMPTY:
      LOG(FATAL) << "Tried to dereference a non-existent parameter.";
  }
}

uint8_t GetParameterValue8Bit(const Parameter& param, const GB_CPU& cpu) {
  return static_cast<uint8_t>(GetParameterValue16Bit(param, cpu));
}
} // namespace

OpcodeExecutor::OpcodeExecutor(std::unique_ptr<memory::MemoryMapper> memory_mapper, 
                               memory::PrimaryFlags* primary_flags,
                               memory::Flag* internal_rom_flag) :
    memory_mapper_(std::move(memory_mapper)),
    bridge_(*memory_mapper_),
    interrupt_enable_(primary_flags->interrupt_enable()),
    interrupt_flag_(primary_flags->interrupt_flag()), 
    internal_rom_flag_(internal_rom_flag) {}

OpcodeExecutor::~OpcodeExecutor() = default;

void OpcodeExecutor::Init() {
  DecompilerFactory factory;
  factory.set_rom(&bridge_);
  factory.set_type(DecompilerFactory::UNFORMATTED_ROM);
  decompiler_ = factory.Build();
  decompiler_->Decompile();
}

void OpcodeExecutor::SwitchToExternalROM() {
  DecompilerFactory factory;
  factory.set_rom(&bridge_);
  factory.set_type(DecompilerFactory::FORMATTED_ROM);
  decompiler_ = factory.Build();
  decompiler_->Decompile();
}

int OpcodeExecutor::ReadInstruction() {
  LOG(INFO) << "Current address: 0x" << std::hex << cpu_.rPC;
  HandleInterrupts();
  if (using_internal_rom_ && internal_rom_flag_->flag()) {
    SwitchToExternalROM();
    LOG(INFO) << "Switched to external ROM.";
    using_internal_rom_ = false;
  }

  Instruction instruction;
  if (!FetchInstruction(cpu_.rPC, &instruction)) {
    LOG(WARNING) << "Invalid address, 0x" << std::hex << cpu_.rPC 
        << ", exiting with error.";
    return -1;
  }
  cpu_.rPC += instruction.instruction_width_bytes;
  OpcodeHandler handler = opcode_map_[instruction.instruction];

  ExecutorContext context;
  context.instruction_ptr = &cpu_.rPC;
  context.interrupt_master_enable = &interrupt_master_enable_;
  context.memory_mapper = memory_mapper_.get();
  context.cpu = &cpu_;

  int handler_result = handler(instruction, &context);
  if (handler_result == -1) {
    return -1;
  } else {
    cpu_.rPC = static_cast<uint16_t>(handler_result);
    LOG(INFO) << "HL = 0x" << std::hex << cpu_.rHL + 0l;
    return instruction.clock_cycles;
  }
}

bool OpcodeExecutor::FetchInstruction(uint16_t address, Instruction* instruction) {
  if (decompiler_->LookUp(address, instruction)) {
    if (instruction->instruction == 0xe9) {
      // 0xe9 is the only instruction which takes a register value as the jump
      // address; thus, the decompiler cannot find this code path in static
      // decompilation. So, if we receive this instruction we must restart the
      // decompiler at this address.
      LOG(INFO) << "Exploring new code path.";
      uint16_t new_code_path = GetParameterValue16Bit(instruction->arg1, cpu_);
      decompiler_->AddPathStart(new_code_path);
      decompiler_->Decompile();
    }
    return true;
  } else {
    LOG(WARNING) << "Could not find valid instruction at given address.";
    return false;
  }
}

// 1) Check interrupt_master_enable_ (IME)
// 2) Check to see if any interrupt flags that are enabled
// 3) Push PC (as if CALL was performed), set PC to interrupt address, disable IME
void OpcodeExecutor::HandleInterrupts() {
  if (interrupt_master_enable_ && CheckInterrupts()) {
    interrupt_master_enable_ = false;
    PushRegister(memory_mapper_.get(), &cpu_, &cpu_.rPC);

    if (interrupt_flag_->v_blank() && interrupt_enable_->v_blank()) {
      LOG(INFO) << "Handling V blank interrupt.";
      interrupt_flag_->set_v_blank(false);
      cpu_.rPC = 0x0040;
    } else if (interrupt_flag_->lcd_stat() && interrupt_enable_->lcd_stat()) {
      LOG(INFO) << "Handling LCD stat interrupt.";
      interrupt_flag_->set_lcd_stat(false);
      cpu_.rPC = 0x0048;
    } else if (interrupt_flag_->timer() && interrupt_enable_->timer()) {
      LOG(INFO) << "Handling timer interrupt.";
      interrupt_flag_->set_timer(false);
      cpu_.rPC = 0x0050;
    } else if (interrupt_flag_->serial() && interrupt_enable_->serial()) {
      LOG(INFO) << "Handling serial interrupt.";
      interrupt_flag_->set_serial(false);
      cpu_.rPC = 0x0058;
    } else if (interrupt_flag_->joypad() && interrupt_enable_->joypad()) {
      LOG(INFO) << "Handling joypad interrupt.";
      interrupt_flag_->set_joypad(false);
      cpu_.rPC = 0x0060;
    }
  }
}

bool OpcodeExecutor::CheckInterrupts() {
  return (interrupt_flag_->v_blank() && interrupt_enable_->v_blank()) ||
      (interrupt_flag_->lcd_stat() && interrupt_enable_->lcd_stat()) ||
      (interrupt_flag_->timer() && interrupt_enable_->timer()) ||
      (interrupt_flag_->serial() && interrupt_enable_->serial()) ||
      (interrupt_flag_->joypad() && interrupt_enable_->joypad());
}

} // namespace handlers
} // namespace back_end
