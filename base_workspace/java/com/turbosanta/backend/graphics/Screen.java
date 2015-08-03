package com.turbosanta.backend.graphics;

import java.awt.Graphics;
import java.awt.image.BufferedImage;
import java.lang.Runnable;
import javax.swing.SwingUtilities;

public class Screen {
  private BufferedImage image;
  private DrawableArea screen;
  private long nativeHandle;

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
      SwingUtilities.invokeAndWait(new Runnable() {
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
    } catch (Exception e) {
      System.out.println(e);
    }
  }
}
