package com.turbosanta.backend.logging.streams;

import java.util.concurrent.atomic.AtomicBoolean;

public abstract class LazyValue<T> {
  AtomicBoolean isComputed = new AtomicBoolean(false);
  Object lock = new Object();
  T value;

  protected abstract T compute();

  /**
   * The idea here is to perform the computation, {@link #compute()} exactly
   * once and then cache the value.
   * 
   * In order to do this, we lock and then only perform the computation if it
   * has not been performed before. This way we know that the condition can only
   * be true for one thread within the synchronized block ever. All other
   * threads that made it past the first check will be stopped until the
   * condition is no longer true.
   *
   * The outer if is just for performance since value is stateless after it is
   * computed.
   *
   * @return result of computation.
   */
  public T get() {
    if (!isComputed.get()) {
      synchronized (lock) {
        if (!isComputed.get()) { // This will only ever be true for one thread.
          value = compute();
          isComputed.set(true);
        }
      }
    }
    return value;
  }
}
