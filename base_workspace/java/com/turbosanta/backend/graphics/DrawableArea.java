package com.turbosanta.backend.graphics;

import java.awt.Graphics;

public interface DrawableArea {
  public Graphics getGraphics();
  public int getWidth();
  public int getHeight();
  public void repaint();
}
