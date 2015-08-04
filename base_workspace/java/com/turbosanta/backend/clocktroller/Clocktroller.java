package com.turbosanta.backend;

import com.turbosanta.backend.graphics.Screen;

public class Clocktroller {
  private Screen screen;
  private long nativeHandle;

  static {
    System.loadLibrary("clocktrollerjni");
  }

  public Clocktroller(Screen screen) {
    this.screen = screen;
  }

  public native void init(byte[] rom, long length);

  public native void run();

  public native void pause();

  public native void kill();

  public native void waitUntilDone();
}
