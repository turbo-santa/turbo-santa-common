package com.turbosanta.backend;

public class Clocktroller {
  private long nativeHandle;

  static {
    System.loadLibrary("clocktrollerjni");
  }

  public native void init(byte[] rom, long length);

  public native void run();

  public native void pause();

  public native void kill();

  public native void waitUntilDone();
}
