package com.turbosanta.backend;

import java.awt.Color;
import java.awt.Graphics;
import java.io.File;
import java.io.FileInputStream;
import javax.swing.JFrame;
import com.turbosanta.backend.graphics.Screen;
import com.turbosanta.backend.graphics.DrawableArea;

import static java.lang.System.out;

public class Main {
  private static class GameFrame extends JFrame {
    private static final long serialVersionUID = 6041544462933292415L;

    public GameFrame() {
      super("Turbo Santa");
      setBackground(Color.WHITE);
      setSize(160 * 3, 144 * 3);
      setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
    }

  }

  private static class FrameWrapper implements DrawableArea {
    private JFrame frame;

    public FrameWrapper(JFrame frame) {
      this.frame = frame;
    }

    @Override
    public Graphics getGraphics() {
      return frame.getGraphics();
    }

    @Override
    public int getHeight() {
      return frame.getHeight();
    }

    @Override
    public int getWidth() {
      return frame.getWidth();
    }
  }

  public static void main(String[] args) throws Exception {
    // if (args.length < 2) {
    //   out.println("Must provide path to ROM file.");
    //   return;
    // }
    GameFrame frame = new GameFrame();
    frame.setVisible(true);
    FrameWrapper wrapper = new FrameWrapper(frame);
    Screen screen = new Screen(wrapper);
    screen.init();

    String romPath = "roms/tetris.gb"; // args[1];
    byte[] rom = readROMFile(romPath);

    Clocktroller clocktroller = new Clocktroller(screen);
    clocktroller.init(rom, rom.length);
    clocktroller.run();
    clocktroller.waitUntilDone();
  }

  public static byte[] readROMFile(String fileName) throws Exception {
    File file = new File(fileName);
    long size = file.length();
    byte[] rom = new byte[(int) size];
    FileInputStream stream = new FileInputStream(file);
    stream.read(rom);
    stream.close();
    return rom;
  }
}
