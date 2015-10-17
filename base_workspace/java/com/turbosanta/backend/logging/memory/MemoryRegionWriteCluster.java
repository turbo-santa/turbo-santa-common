package com.turbosanta.backend.logging.memory;

import java.util.List;

import com.google.common.collect.ImmutableList;

public class MemoryRegionWriteCluster {
  private List<MemoryRegionWriteEvent> memoryRegionWriteEvents;

  public MemoryRegionWriteCluster(List<MemoryRegionWriteEvent> memoryRegionWriteEvents) {
    this.memoryRegionWriteEvents = ImmutableList.copyOf(memoryRegionWriteEvents);
  }

  public List<MemoryRegionWriteEvent> getMemoryRegionWriteEvents() {
    return memoryRegionWriteEvents;
  }
}
