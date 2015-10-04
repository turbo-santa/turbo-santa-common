package com.turbosanta.backend.logging.memory;

import com.turbosanta.backend.logging.ConsumerBuilder;
import com.turbosanta.backend.logging.Messages.Message;
import com.turbosanta.backend.logging.streams.InStream;

public class MemoryProfilerConsumerBuilder 
    extends ConsumerBuilder<MemoryProfilerConsumerBuilder, MemoryProfilerConsumer> {
  private InStream<Message> stream;

  @Override
  public MemoryProfilerConsumerBuilder setMessageStream(InStream<Message> stream) {
    this.stream = stream;
    return this;
  }

  @Override
  public MemoryProfilerConsumer build() {
    return new MemoryProfilerConsumer(stream);
  }
}
