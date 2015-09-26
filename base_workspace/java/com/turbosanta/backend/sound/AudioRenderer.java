package com.turbosanta.backend.sound;

import javax.sound.sampled.*;
import java.nio.ByteOrder;

public class AudioRenderer {

  private static final float SAMPLE_RATE = 44100.0f; // Hz
  private static final int SAMPLE_SIZE = 8; // bits
  private static final int CHANNELS = 2;

  public static void testAudio() throws LineUnavailableException, InterruptedException {
    AudioController ac = new AudioController(3);
    ac.init();
    int startingFrequency = 440;
    for (int r = 1; r < 5; r++) {
      for (int i = 1; i < 10; i++) {
        double exp1 = ((double) i - 1) / 12d;
        double exp2 = ((double) i + 3) / 12d;
        byte[] soundBytes = frequencyToBytes((int) (startingFrequency * Math.pow(2d, exp1)), (int) (startingFrequency * Math.pow(2d, exp2)), 0.1);
        ac.submitAudioChunk(soundBytes);
      }
      for (int i = 10; i > 0; i--) {
        double exp1 = ((double) i - 1) / 12d;
        double exp2 = ((double) i + 3) / 12d;
        byte[] soundBytes = frequencyToBytes((int) (startingFrequency * Math.pow(2d, exp2)), (int) (startingFrequency * Math.pow(2d, exp1)), 0.1);
        ac.submitAudioChunk(soundBytes);
      }
    }
    ac.tearDown();
  }

  public static void playSomeSound() throws LineUnavailableException {
    Line.Info[] infos = AudioSystem.getSourceLineInfo(new Line.Info(SourceDataLine.class));
    for (Line.Info in : infos) {
      if (in instanceof DataLine.Info) {
        AudioFormat[] forms = ((DataLine.Info) in).getFormats();
        for (int n = 0; n < forms.length; ++n) {
          System.err.println(forms[n].toString());
        }
      }
    }

    AudioFormat audioFormat = new AudioFormat(SAMPLE_RATE, SAMPLE_SIZE, CHANNELS, true, ByteOrder.nativeOrder() == ByteOrder.BIG_ENDIAN);
    SourceDataLine line = AudioSystem.getSourceDataLine(audioFormat);
    line.open(audioFormat, (int) SAMPLE_RATE);
    line.start();

    int startingFrequency = 440;
    for (int r = 1; r < 5; r++) {
      for (int i = 1; i < 10; i++) {
        double exp1 = ((double) i - 1) / 12d;
        double exp2 = ((double) i + 3) / 12d;
        byte[] soundBytes = frequencyToBytes((int) (startingFrequency * Math.pow(2d, exp1)), (int) (startingFrequency * Math.pow(2d, exp2)), 0.1 * r);
        line.write(soundBytes, 0, soundBytes.length);
      }
      for (int i = 10; i > 0; i--) {
        double exp1 = ((double) i - 1) / 12d;
        double exp2 = ((double) i + 3) / 12d;
        byte[] soundBytes = frequencyToBytes((int) (startingFrequency * Math.pow(2d, exp2)), (int) (startingFrequency * Math.pow(2d, exp1)), 0.2 * r);
        line.write(soundBytes, 0, soundBytes.length);
      }
    }
    line.drain();
    line.close();
    System.exit(0);
  }

  private static byte[] frequencyToBytes(int frequency1, int frequency2, double length) {
    byte[] soundBytes = new byte[(int) (length * SAMPLE_RATE)];

    for (int i = 0; i < soundBytes.length; i += 2) {
      double period1 = (double) SAMPLE_RATE / frequency1;
      double angle1 = (2.0 * Math.PI * i) / period1;
      soundBytes[i] = (byte) (Math.sin(angle1) * 127f);
      double period2 = (double) SAMPLE_RATE / frequency2;
      double angle2 = (2.0 * Math.PI * i) / period2;
      soundBytes[i + 1] = (byte) (Math.sin(angle2) * 127f);
    }
    return soundBytes;
  }
}
