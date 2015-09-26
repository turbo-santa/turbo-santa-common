package com.turbosanta.backend.logging.streams;

import com.google.common.base.Optional;

public interface InStream<T> {
  Optional<T> take();
  boolean isClosed();
}
