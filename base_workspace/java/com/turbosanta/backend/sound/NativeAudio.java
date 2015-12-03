package com.turbosanta.backend.sound;

import java.util.Arrays;

public class NativeAudio {

  private AudioController audioController;
  private long nativeHandle;

  static {
    System.loadLibrary("soundjni");
  }

  public NativeAudio(AudioController audioController) {
    this.audioController = audioController;
  }

  public native void init();

  // Called from JNI.
  public void submitAudioChunk(byte[] data) {
    //System.err.println("NativeAudio: submitting audio chunk: " + data.length + "\n" + Arrays.toString(data));
    audioController.submitAudioChunk(Arrays.copyOfRange(data, 0, data.length));
  }

}
