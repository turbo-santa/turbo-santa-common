package com.turbosanta.backend.logging.memory;

import com.turbosanta.backend.logging.Messages.Message;
import com.turbosanta.backend.logging.streams.InStream;
import com.turbosanta.backend.logging.streams.InStreamManipulators.Process;
import com.turbosanta.backend.logging.streams.StringPrinterSinkProcess;

public class MemoryRegionWriteEventPrintingPipeline implements Process<Message, Void> {
  private static final String LOG_NAME = "memory_region_write_event.log";
  private final int regionMin;
  private final int regionMax;

  public MemoryRegionWriteEventPrintingPipeline(int regionMin, int regionMax) {
    this.regionMin = regionMin;
    this.regionMax = regionMax;
  }

  @Override
  public InStream<Void> apply(InStream<Message> inputStream) {
    return inputStream
      .applyProcess(new MemoryEventContextFilteringProcess())
      .applyProcess(new MemoryRegionWriteEventFilteringProcess(regionMin, regionMax))
      .map((event) -> 
        "{ " + 
        event.getWriteEvent().getProgramCounter() + 
        ", " + 
        event.getDistanceToPreviousMemoryRegionWriteEvent() + 
        " }"
      )
      .applyProcess(new StringPrinterSinkProcess(LOG_NAME));
  }
}
