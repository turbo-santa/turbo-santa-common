package com.turbosanta.backend;

import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.Dimension;
import java.awt.event.KeyAdapter;
import java.awt.event.KeyEvent;
import java.awt.Graphics;

import javax.swing.JFrame;
import javax.swing.JPanel;

import com.turbosanta.backend.graphics.DrawableArea;
import com.turbosanta.backend.sound.AudioController;

import static java.lang.System.out;

import static com.turbosanta.backend.Backend.BackendFactory;
import static com.turbosanta.backend.Backend.ButtonEventListener;
import static com.turbosanta.backend.BackendUtils.readROMFile;

public class Main {
  private static class GameFrame extends JFrame {
    private static final long serialVersionUID = 6041544462933292415L;
    private JPanel content = new JPanel();

    public GameFrame() {
      super("Turbo Santa");
      setBackground(Color.WHITE);
      setLayout(new BorderLayout());
      content.setPreferredSize(new Dimension(160 * 3, 144 * 3));
      add(content, BorderLayout.CENTER);
      pack();
      setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
    }

    public JPanel getContent() {
      return content;
    }
  }

  private static class FrameWrapper implements DrawableArea {
    private JPanel frame;

    public FrameWrapper(GameFrame frame) {
      this.frame = frame.getContent();
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

    @Override
    public void repaint() {
      frame.repaint();
    }
  }

  public static class InputAdapter extends KeyAdapter {
    private ButtonEventListener listener;

    public InputAdapter(ButtonEventListener listener) {
      this.listener = listener;
    }

    @Override
    public void keyPressed(KeyEvent event) {
      ButtonType type = translateToGameboy(event);
      if (type != null) {
        listener.buttonPressed(type);
      }
    }

    @Override
    public void keyReleased(KeyEvent event) {
      ButtonType type = translateToGameboy(event);
      if (type != null) {
        listener.buttonReleased(type);
      }
    }

    public ButtonType translateToGameboy(KeyEvent event) {
      switch (event.getKeyChar()) {
        case 's':
          return ButtonType.DOWN;
        case 'w':
          return ButtonType.UP;
        case 'a':
          return ButtonType.LEFT;
        case 'd':
          return ButtonType.RIGHT;
        case 'i':
          return ButtonType.START;
        case 'u':
          return ButtonType.SELECT;
        case 'j':
          return ButtonType.A;
        case 'k':
          return ButtonType.B;
        default:
          return null;
      }
    }
  }

  public static void main(String[] args) throws Exception {
    if (args.length < 2) {
      out.println("Must provide path to ROM file.");
      return;
    }
    String romPath = args[1];

    BackendFactory factory = new BackendFactory();
    factory.setROM(readROMFile(romPath));

    GameFrame frame = new GameFrame();
    frame.setVisible(true);
    factory.setDrawableArea(new FrameWrapper(frame));

    factory.setAudioPlayer(new AudioController(3));

    Backend backend = factory.build();
    backend.run();
    frame.addKeyListener(new InputAdapter(backend.getButtonEventListener()));
  }

}
