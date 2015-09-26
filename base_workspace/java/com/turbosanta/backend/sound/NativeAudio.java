package com.turbosanta.backend.sound;

public class NativeAudio {

  private AudioController audioController;
  private long nativeHandle;

  public NativeAudio(AudioController audioController) {
    this.audioController = audioController;
  }

  public native void init();

  public void submitAudioChunk(byte[] data) {
    audioController.submitAudioChunk(data);
  }

}
