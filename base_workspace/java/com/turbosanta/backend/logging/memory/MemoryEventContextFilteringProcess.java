package com.turbosanta.backend.logging.memory;

import com.google.common.base.Optional;
import com.turbosanta.backend.logging.Messages.MemoryMessage;
import com.turbosanta.backend.logging.Messages.Message;
import com.turbosanta.backend.logging.Messages.OpcodeExecutionInfo;
import com.turbosanta.backend.logging.Messages.OpcodeMessage;
import com.turbosanta.backend.logging.streams.InStream;
import com.turbosanta.backend.logging.streams.InStreamManipulators.Process;

import static com.turbosanta.backend.logging.memory.MemoryEvent.convertToMemoryEvent;

public class MemoryEventContextFilteringProcess implements Process<Message, MemoryEventContext<MemoryEvent>> {
  private OpcodeExecutionInfo executionInfo = OpcodeExecutionInfo.getDefaultInstance();

  @Override
  public InStream<MemoryEventContext<MemoryEvent>> apply(InStream<Message> inputStream) {
    return inputStream.flatMap((Message message) -> filterMemoryEvents(message));
  }

  private Optional<MemoryEventContext<MemoryEvent>> filterMemoryEvents(Message message) {
    switch (message.getPayloadCase()) {
      case MEMORY_MESSAGE:
        MemoryMessage memoryMessage = message.getMemoryMessage();
        if (memoryMessage.getPayloadCase().equals(MemoryMessage.PayloadCase.MEMORY_ACCESS)) {
          MemoryEvent memoryEvent = convertToMemoryEvent(memoryMessage.getMemoryAccess());
          return Optional.of(new MemoryEventContext<>(memoryEvent, executionInfo.getProgramCounter()));
        }
      case OPCODE_MESSAGE:
        OpcodeMessage opcodeMessage = message.getOpcodeMessage();
        if (opcodeMessage.getPayloadCase().equals(OpcodeMessage.PayloadCase.OPCODE_EXECUTION_INFO)) {
          executionInfo = opcodeMessage.getOpcodeExecutionInfo();
        }
        // Fall through expected.
      default:
        return Optional.absent();
    }
  }
}
