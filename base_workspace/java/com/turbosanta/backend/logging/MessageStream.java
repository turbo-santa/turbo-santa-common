package com.turbosanta.backend.logging;

import com.google.common.base.Optional;
import com.google.protobuf.Message;

public interface MessageStream<T extends Message> {
  /**
   * Will block until a message is available or stream closes.
   */
  Optional<T> take();

  boolean isClosed();
}
