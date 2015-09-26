package com.turbosanta.backend.logging.streams;

public interface OutStream<T> {
  void put(T value);
  void close();
}
