package com.turbosanta.backend.logging.memory;

public class MemoryEventContext<T extends MemoryEvent> {
  private T memoryEvent;
  private int programCounter;

  public MemoryEventContext(T memoryEvent, int programCounter) {
    this.memoryEvent = memoryEvent;
    this.programCounter = programCounter;
  }

  public T getMemoryEvent() {
    return memoryEvent;
  }

  public int getProgramCounter() {
    return programCounter;
  }
}
