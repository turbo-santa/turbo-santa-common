package com.turbosanta.backend.logging.memory;

import com.google.common.base.Optional;
import com.turbosanta.backend.logging.Consumer;
import com.turbosanta.backend.logging.Messages.MemoryAccess;
import com.turbosanta.backend.logging.Messages.Message;
import com.turbosanta.backend.logging.streams.InStream;

public class MemoryProfilerConsumer implements Consumer {
  private InStream<Message> stream;

  public MemoryProfilerConsumer(InStream<Message> stream) {
    this.stream = stream;
  }

  @Override
  public void run() {
    while (true) {
      Optional<Message> optionalMessage = stream.take();
      if (optionalMessage.isPresent()) {
        MemoryAccess memoryAccess = optionalMessage.get().getMemoryMessage().getMemoryAccess();
        if (memoryAccess.getAccessType() == MemoryAccess.AccessType.READ) {
          System.out.println(
              "READ, address = " + memoryAccess.getAddress() 
              + ", value = " + memoryAccess.getOldValue());
        } else {
          System.out.println(
              "WRITE, address = " + memoryAccess.getAddress() 
              + ", old value = " + memoryAccess.getOldValue() 
              + "new value = " + memoryAccess.getNewValue());
        }
      }
    }
  }
}
