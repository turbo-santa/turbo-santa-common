package com.turbosanta.backend.logging;

public interface Consumer extends Runnable {
  @Override
  void run();
}
