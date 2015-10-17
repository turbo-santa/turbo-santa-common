package com.turbosanta.backend.logging.streams;

import com.turbosanta.backend.logging.Consumer;

public class StreamRunner implements Consumer {
  private InStream<Void> stream;

  public StreamRunner(InStream<Void> stream) {
    this.stream = stream;
  }

  @Override
  public void run() {
    while (!stream.isClosed()) {
      stream.take();
    }
  }
}
