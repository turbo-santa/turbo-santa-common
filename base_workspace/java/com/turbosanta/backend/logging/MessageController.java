package com.turbosanta.backend.logging;

public class MessageController {
  private static CommunicationLayer communicationLayer = null;

  static {
    System.loadLibrary("message_controllerjni");
  }

  public CommunicationLayer getCommunicationLayer() {
    if (communicationLayer == null) {
      communicationLayer = new CommunicationLayer(getCommunicationLayerPointer());
    }
    return communicationLayer;
  }

  private native long getCommunicationLayerPointer();
}
