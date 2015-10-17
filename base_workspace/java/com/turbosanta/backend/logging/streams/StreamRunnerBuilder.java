package com.turbosanta.backend.logging.streams;

import com.turbosanta.backend.logging.ConsumerBuilder;
import com.turbosanta.backend.logging.Messages.Message;
import com.turbosanta.backend.logging.streams.InStreamManipulators.Process;

public class StreamRunnerBuilder extends ConsumerBuilder<StreamRunnerBuilder, StreamRunner> {
  private InStream<Message> stream;
  private Process<Message, Void> sinkProcess;

  public StreamRunnerBuilder(Process<Message, Void> sinkProcess) {
    this.sinkProcess = sinkProcess;
  }

  @Override
  public StreamRunnerBuilder setMessageStream(InStream<Message> stream) {
    this.stream = stream;
    return this;
  }

  @Override
  public StreamRunner build() {
    return new StreamRunner(sinkProcess.apply(stream));
  }
}
