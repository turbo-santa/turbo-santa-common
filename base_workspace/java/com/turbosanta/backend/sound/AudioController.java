package com.turbosanta.backend.sound;

import javax.sound.sampled.AudioFormat;
import javax.sound.sampled.AudioSystem;
import javax.sound.sampled.LineUnavailableException;
import javax.sound.sampled.SourceDataLine;
import java.nio.ByteOrder;

public class AudioController {

  private static final float SAMPLE_RATE = 44100.0f; // Hz
  private static final int SAMPLE_SIZE = 8; // bits
  private static final int CHANNELS = 2;

  private SourceDataLine sourceLine;
  private SoundBuffer buffer;
  private Thread audioThread;

  private boolean shouldPlayAudio = true;

  public AudioController(int bufferSize) {
    buffer = new SoundBuffer(bufferSize);
  }

  /**
   * Called from JNI
   */
  public void submitAudioChunk(byte[] audioChunk) {
    buffer.queue(audioChunk);
  }

  public void init() throws LineUnavailableException {
    AudioFormat audioFormat = new AudioFormat(SAMPLE_RATE, SAMPLE_SIZE, CHANNELS, true, ByteOrder.nativeOrder() == ByteOrder.BIG_ENDIAN);
    sourceLine = AudioSystem.getSourceDataLine(audioFormat);
    sourceLine.open(audioFormat, (int) SAMPLE_RATE);
    sourceLine.start();

    audioThread = new AudioThread();
    audioThread.start();
  }

  public void tearDown() {
    System.err.println("AudioController: tearing down Audio Thread...");
    shouldPlayAudio = false;
    audioThread.interrupt();
    try {
      System.err.println("AudioController: joining Audio Thread...");
      audioThread.join();
    } catch (InterruptedException e) {
      e.printStackTrace();
    }
    System.err.println("AudioController: tearing down source line...");
    sourceLine.drain();
    sourceLine.close();
    buffer = null;
  }

  private class AudioThread extends Thread {

    private AudioThread() {
      super("Audio Thread");
    }

    @Override
    public void run() {
      System.err.println("Audio Thread spinning up...");
      while (shouldPlayAudio) {
        byte[] sample = new byte[sourceLine.getBufferSize()];
        buffer.fill(sample, 0, sample.length);
        sourceLine.write(sample, 0, sample.length);
      }
    }
  }
}
