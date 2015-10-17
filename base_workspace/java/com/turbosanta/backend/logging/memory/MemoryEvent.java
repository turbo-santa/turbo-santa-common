package com.turbosanta.backend.logging.memory;

import com.turbosanta.backend.logging.Messages.MemoryAccess;
import com.turbosanta.backend.logging.Messages.MemoryAccess.AccessType;

import static com.google.common.base.Preconditions.checkArgument;

public abstract class MemoryEvent {
  private final int address;

  private MemoryEvent(int address) {
    this.address = address;
  }

  public int getAddress() {
    return address;
  }

  public static MemoryEvent convertToMemoryEvent(MemoryAccess memoryAccess) {
    switch (memoryAccess.getAccessType()) {
      case READ:
        return ReadEvent.convertToReadEvent(memoryAccess);
      case WRITE:
        return WriteEvent.convertToWriteEvent(memoryAccess);
      default:
        throw new IllegalArgumentException("Expected either READ or WRITE, but found: " 
            + memoryAccess.getAccessType());
    }
  }

  public static class ReadEvent extends MemoryEvent {
    private final int value;

    private ReadEvent(int address, int value) {
      super(address);
      this.value = value;
    }

    public int getValue() {
      return value;
    }

    public static ReadEvent convertToReadEvent(MemoryAccess memoryAccess) {
      checkArgument(memoryAccess.getAccessType() == AccessType.READ);
      return new ReadEvent(memoryAccess.getAddress(), memoryAccess.getOldValue());
    }
  }

  public static class WriteEvent extends MemoryEvent {
    private final int oldValue;
    private final int newValue;

    private WriteEvent(int address, int oldValue, int newValue) {
      super(address);
      this.oldValue = oldValue;
      this.newValue = newValue;
    }

    public int getOldValue() {
      return oldValue;
    }

    public int getNewValue() {
      return newValue;
    }

    public static WriteEvent convertToWriteEvent(MemoryAccess memoryAccess) {
      checkArgument(memoryAccess.getAccessType() == AccessType.WRITE);
      return new WriteEvent(memoryAccess.getAddress(), 
          memoryAccess.getOldValue(), 
          memoryAccess.getNewValue());
    }
  }
}
