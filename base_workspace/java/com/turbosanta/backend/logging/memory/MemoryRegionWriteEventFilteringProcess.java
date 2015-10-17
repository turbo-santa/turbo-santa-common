package com.turbosanta.backend.logging.memory;

import com.google.common.base.Optional;
import com.turbosanta.backend.logging.memory.MemoryEvent.WriteEvent;
import com.turbosanta.backend.logging.streams.InStream;
import com.turbosanta.backend.logging.streams.InStreamManipulators.Process;

public class MemoryRegionWriteEventFilteringProcess
    implements Process<MemoryEventContext<MemoryEvent>, MemoryRegionWriteEvent> {
  private final int regionMin;
  private final int regionMax;

  private int distanceToPreviousOAMWriteEvent = 0;

  public MemoryRegionWriteEventFilteringProcess(int regionMin, int regionMax) {
    this.regionMin = regionMin;
    this.regionMax = regionMax;
  }

  @Override
  public InStream<MemoryRegionWriteEvent> apply(InStream<MemoryEventContext<MemoryEvent>> inputStream) {
    return inputStream.flatMap((MemoryEventContext<MemoryEvent> eventContext) -> findWriteEvents(eventContext));
  }

  private Optional<MemoryRegionWriteEvent> findWriteEvents(MemoryEventContext<?> memoryEventContext) {
    MemoryEvent memoryEvent = memoryEventContext.getMemoryEvent();
    if (regionMin <= memoryEvent.getAddress() && memoryEvent.getAddress() <= regionMax) {
      if (memoryEvent instanceof WriteEvent) {
        int temp = distanceToPreviousOAMWriteEvent;
        distanceToPreviousOAMWriteEvent = 0;
        @SuppressWarnings("unchecked") MemoryRegionWriteEvent memoryRegionWriteEvent =
            new MemoryRegionWriteEvent((MemoryEventContext<WriteEvent>) memoryEventContext, temp);
        return Optional.of(memoryRegionWriteEvent);
      }
    }
    distanceToPreviousOAMWriteEvent++;
    return Optional.absent();
  }
}
