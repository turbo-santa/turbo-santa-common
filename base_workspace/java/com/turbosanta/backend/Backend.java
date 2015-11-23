package com.turbosanta.backend;

import com.turbosanta.backend.clocktroller.Clocktroller;
import com.turbosanta.backend.graphics.Screen;
import com.turbosanta.backend.graphics.DrawableArea;
import com.turbosanta.backend.joypad.Joypad;
import com.turbosanta.backend.sound.AudioController;
import com.turbosanta.backend.sound.NativeAudio;

import javax.sound.sampled.LineUnavailableException;

public class Backend {
  private Clocktroller clocktroller;
  private Joypad joypad;

  private Backend(Clocktroller clocktroller, Joypad joypad) {
    this.clocktroller = clocktroller;
    this.joypad = joypad;
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

  public ButtonEventListener getButtonEventListener() {
    return new ButtonEventListener(this);
  }

  public static class ButtonEventListener {
    private Backend backend;

    private ButtonEventListener(Backend backend) {
      this.backend = backend;
    }

    public void buttonPressed(ButtonType type) {
      switch (type) {
        case DOWN:
          backend.joypad.setDown(true);
          break;
        case UP:
          backend.joypad.setUp(true);
          break;
        case LEFT:
          backend.joypad.setLeft(true);
          break;
        case RIGHT:
          backend.joypad.setRight(true);
          break;
        case START:
          backend.joypad.setStart(true);
          break;
        case SELECT:
          backend.joypad.setSelect(true);
          break;
        case A:
          backend.joypad.setA(true);
          break;
        case B:
          backend.joypad.setB(true);
          break;
      }
    }

    public void buttonReleased(ButtonType type) {
      switch (type) {
        case DOWN:
          backend.joypad.setDown(false);
          break;
        case UP:
          backend.joypad.setUp(false);
          break;
        case LEFT:
          backend.joypad.setLeft(false);
          break;
        case RIGHT:
          backend.joypad.setRight(false);
          break;
        case START:
          backend.joypad.setStart(false);
          break;
        case SELECT:
          backend.joypad.setSelect(false);
          break;
        case A:
          backend.joypad.setA(false);
          break;
        case B:
          backend.joypad.setB(false);
          break;
      }
    }
  }

  public static class BackendFactory {
    private DrawableArea drawableArea;
    private AudioController audioController;
    private byte[] rom;

    public BackendFactory setDrawableArea(DrawableArea drawableArea) {
      this.drawableArea = drawableArea;
      return this;
    }

    public BackendFactory setROM(byte[] rom) {
      this.rom = rom;
      return this;
    }

    public BackendFactory setAudioPlayer(AudioController audioController) {
      this.audioController = audioController;
      return this;
    }

    public Backend build() {
      Screen screen = new Screen(drawableArea);
      screen.init();

      NativeAudio nativeAudio = new NativeAudio(audioController);
      nativeAudio.init();
      try {
        audioController.init();
      } catch (LineUnavailableException e) {
        e.printStackTrace();
      }

      Clocktroller clocktroller = new Clocktroller(screen, nativeAudio);
      clocktroller.init(rom, rom.length);

      Joypad joypad = new Joypad();
      joypad.init(clocktroller);

      return new Backend(clocktroller, joypad);
    }
  }
}
