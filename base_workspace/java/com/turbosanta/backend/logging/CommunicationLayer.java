package com.turbosanta.backend.logging;

import com.google.common.base.Optional;
import com.google.protobuf.InvalidProtocolBufferException;
import com.turbosanta.backend.logging.Messages.Message;
import com.turbosanta.backend.logging.Messages.MessageOption;

public class CommunicationLayer {
  private long nativeHandle;

  public CommunicationLayer(long nativeHandle) {
    this.nativeHandle = nativeHandle;
  }

  public Optional<Message> takeMessage() throws InvalidProtocolBufferException {
    MessageOption option = MessageOption.parseFrom(takeMessageInternal());
    if (option.hasPayload()) {
      return Optional.of(option.getPayload());
    } else {
      return Optional.absent();
    }
  }

  private native byte[] takeMessageInternal();

  public native boolean isClosed();
}
