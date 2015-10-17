package com.turbosanta.backend.logging.memory;

import com.turbosanta.backend.logging.memory.MemoryEvent.WriteEvent;

public class MemoryRegionWriteEvent {
  private int distanceToMemoryRegionWriteEvent;
  private MemoryEventContext<WriteEvent> writeEvent;

  public MemoryRegionWriteEvent(MemoryEventContext<WriteEvent> writeEvent, int distanceToMemoryRegionWriteEvent) {
    this.distanceToMemoryRegionWriteEvent = distanceToMemoryRegionWriteEvent;
    this.writeEvent = writeEvent;
  }

  /**
   * @return the distanceToMemoryRegionWriteEvent
   */
  public int getDistanceToPreviousMemoryRegionWriteEvent() {
    return distanceToMemoryRegionWriteEvent;
  }

  /**
   * @return the writeEvent
   */
  public MemoryEventContext<WriteEvent> getWriteEvent() {
    return writeEvent;
  }
}
