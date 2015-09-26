package com.turbosanta.backend.clocktroller;

import com.turbosanta.backend.graphics.Screen;
import com.turbosanta.backend.sound.NativeAudio;

public class Clocktroller {
  private Screen screen;
  private NativeAudio audioPlayer;
  private long nativeHandle;

  static {
    System.loadLibrary("clocktrollerjni");
  }

  public Clocktroller(Screen screen, NativeAudio audioPlayer) {
    this.screen = screen;
    this.audioPlayer = audioPlayer;
  }

  public native void init(byte[] rom, long length);

  public native void run();

  public native void pause();

  public native void kill();
}
