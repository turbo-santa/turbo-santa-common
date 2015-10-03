package com.turbosanta.backend.logging.memory;

import com.turbosanta.backend.logging.AbstractConsumerFactory;
import com.turbosanta.backend.logging.Messages.Message;
import com.turbosanta.backend.logging.streams.InStream;

public class MemoryProfilerConsumerFactory 
    extends AbstractConsumerFactory<MemoryProfilerConsumerFactory, MemoryProfilerConsumer> {
  private InStream<Message> stream;

  @Override
  public MemoryProfilerConsumerFactory setMessageStream(InStream<Message> stream) {
    this.stream = stream;
    return this;
  }

  @Override
  public MemoryProfilerConsumer build() {
    return new MemoryProfilerConsumer(stream);
  }
}
