package com.turbosanta.backend.joypad;

import com.turbosanta.backend.clocktroller.Clocktroller;

public class Joypad {
  private long nativeHandle;

  static {
    System.loadLibrary("joypadjni");
  }

  public native void init(Clocktroller clocktroller);

  public native void setDown(boolean isPushed);

  public native void setUp(boolean isPushed);

  public native void setLeft(boolean isPushed);

  public native void setRight(boolean isPushed);

  public native void setStart(boolean isPushed);
  
  public native void setSelect(boolean isPushed);

  public native void setB(boolean isPushed);

  public native void setA(boolean isPushed);
}
