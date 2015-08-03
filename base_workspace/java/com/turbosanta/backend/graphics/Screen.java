package com.turbosanta.backend.graphics;

import java.awt.Graphics;
import java.awt.image.BufferedImage;
import java.io.File;
import java.lang.Runnable;
import javax.imageio.ImageIO;
import javax.swing.SwingUtilities;

public class Screen {
  private BufferedImage image;
  private DrawableArea screen;
  private long nativeHandle;
  private int time = 0;

  static {
    System.loadLibrary("screenjni");
  }

  public Screen(DrawableArea screen) {
    this.screen = screen;
    image = new BufferedImage(
        getWidth(), 
        getHeight(), 
        BufferedImage.TYPE_BYTE_GRAY
    );
  }

  public native void init();

  public static native int getWidth();

  public static native int getHeight();

  public void draw() {
    image.flush();
    try {
      File file = new File("image" + time++ + ".png");
      ImageIO.write(image, "png", file);
    } catch (Exception e) {}
    SwingUtilities.invokeLater(new Runnable() {
      @Override
      public void run() {
        Graphics graphics = screen.getGraphics();
        graphics.drawImage(
            image, 
            0, 0, screen.getWidth(), screen.getHeight(), 
            0, 0, image.getWidth(), image.getHeight(), 
            null
            );
      }
    });
  }
}
