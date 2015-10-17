package com.turbosanta.backend.logging;

import java.util.List;

import com.google.common.base.Optional;
import com.google.common.collect.Lists;
import com.turbosanta.backend.logging.memory.MemoryRegionWriteEventPrintingPipeline;
import com.turbosanta.backend.logging.Messages.Message;
import com.turbosanta.backend.logging.streams.InStream;
import com.turbosanta.backend.logging.streams.MultiTraversalStream;
import com.turbosanta.backend.logging.streams.StreamRunnerBuilder;

public class ConsumerController {
  private static ConsumerController consumerController = new ConsumerController();
  private final MessageController messageController;
  private List<ConsumerBuilder<?, ? extends Consumer>> consumerFactories;

  private ConsumerController() {
    consumerFactories = Lists.newArrayList();
    consumerFactories.add(new StreamRunnerBuilder(new MemoryRegionWriteEventPrintingPipeline(0x9800, 0x9FFF)));
    messageController = new MessageController();
  }

  public static ConsumerController instance() {
    return consumerController;
  }

  // TODO: This should launch all consumers and should split the input from the
  // communication layer so that multiple consumers can coexist.
  public void startConsumers() {
    MultiTraversalStream<Message> parentStream = MultiTraversalStream.newMultiTraversalStream(new InStream<Message>() {
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
    for (ConsumerBuilder<?, ? extends Consumer> factory : consumerFactories) {
      factory.setMessageStream(parentStream.newIndependentStream());
      new Thread(factory.build()).run();
    }
  }
}
