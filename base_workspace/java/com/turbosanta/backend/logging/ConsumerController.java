package com.turbosanta.backend.logging;

import java.util.List;

import com.google.common.base.Optional;
import com.google.common.collect.Lists;
import com.turbosanta.backend.logging.Messages.Message;
import com.turbosanta.backend.logging.streams.InStream;

public class ConsumerController {
  private static ConsumerController consumerController = new ConsumerController();
  private final MessageController messageController;
  private List<AbstractConsumerFactory<?, ? extends Consumer>> consumerFactories = Lists.newArrayList();

  private ConsumerController() {
    messageController = new MessageController();
  }

  public static ConsumerController instance() {
    return consumerController;
  }

  // TODO: This should launch all consumers and should split the input from the
  // communication layer so that multiple consumers can coexist.
  public void startConsumers() {
    AbstractConsumerFactory<?, ?> factory = consumerFactories.get(0);
    factory.setMessageStream(new InStream<Message>() {
      private final CommunicationLayer communicationLayer = messageController.getCommunicationLayer();

      @Override
      public Optional<Message> take() {
        try {
          return communicationLayer.takeMessage();
        } catch (Exception e) {
          return Optional.absent();
        }
      }

      @Override
      public boolean isClosed() {
        return communicationLayer.isClosed();
      }
    });
    Consumer consumer = factory.build();
    new Thread(consumer).run();
  }
}
