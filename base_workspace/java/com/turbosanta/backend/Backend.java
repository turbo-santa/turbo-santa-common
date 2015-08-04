package com.turbosanta.backend;

import com.turbosanta.backend.clocktroller.Clocktroller;
import com.turbosanta.backend.graphics.Screen;
import com.turbosanta.backend.graphics.DrawableArea;

public class Backend {
  private Clocktroller clocktroller;

  private Backend(Clocktroller clocktroller) {
    this.clocktroller = clocktroller;
  }

  public void run() {
    clocktroller.run();
  }

  public void pause() {
    clocktroller.pause();
  }

  public void kill() {
    clocktroller.kill();
  }

  public static class BackendFactory {
    private DrawableArea drawableArea;
    private byte[] rom;

    public BackendFactory setDrawableArea(DrawableArea drawableArea) {
      this.drawableArea = drawableArea;
      return this;
    }

    public BackendFactory setROM(byte[] rom) {
      this.rom = rom;
      return this;
    }

    public Backend build() {
      Screen screen = new Screen(drawableArea);
      screen.init();

      Clocktroller clocktroller = new Clocktroller(screen);
      clocktroller.init(rom, rom.length);

      return new Backend(clocktroller);
    }
  }
}
