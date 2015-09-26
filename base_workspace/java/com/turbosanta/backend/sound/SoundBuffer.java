package com.turbosanta.backend.sound;

import javax.sound.sampled.Line;
import java.nio.ByteBuffer;
import java.util.LinkedList;

public class SoundBuffer {

  private LinkedList<SoundData> buffer;
  private int maxBuffers;

  public SoundBuffer(int maxBuffers) {
    this.maxBuffers = maxBuffers;
    buffer = new LinkedList<>();
  }

  public synchronized void queue(byte[] data) {
    buffer.addLast(new SoundData(data));
    if (buffer.size() > maxBuffers) {
      System.err.println("Audio buffer is full, dropping first sample!");
      buffer.removeFirst();
      maxBuffers *= 2;
      System.err.println("Increasing buffer size to: " + maxBuffers);
    }
    notify();
  }

  public synchronized int fill(byte[] data, int offset, int length) {
    int filled = 0;

    ByteBuffer byteBuffer = ByteBuffer.wrap(data);
    byteBuffer.position(offset);
    if (buffer.isEmpty()) {
      try {
        wait();
      } catch (InterruptedException e) {
        e.printStackTrace();
      }
    }
    while (length > 0 && !buffer.isEmpty()) {
      SoundData buff = buffer.getFirst();
      if (buff.data.length > length) {
        break;
      }

      byteBuffer.put(buff.data, 0, buff.data.length);
      length -= buff.data.length;
      filled += buff.data.length;
      buffer.removeFirst();
    }

    return filled;
  }

  public int size() {
    return buffer.size();
  }

  private static class SoundData {
    byte[] data;

    private SoundData(byte[] data) {
      this.data = data;
    }
  }
}
