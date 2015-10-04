package com.turbosanta.backend.logging.streams;

import com.google.common.base.Optional;

import javax.annotation.Nullable;

public class MultiTraversalStream<T> implements InStream<T> {
  private InStream<T> baseStream;
  private StreamNode<T> streamNode;

  private MultiTraversalStream(InStream<T> baseStream, StreamNode<T> streamNode) {
    this.baseStream = baseStream;
    this.streamNode = streamNode;
  }

  private MultiTraversalStream(InStream<T> baseStream) {
    this.baseStream = baseStream;
    this.streamNode = new StreamNode<>(null, new StreamNodeLazyValue());
  }

  public static <T> MultiTraversalStream<T> newMultiTraversalStream(InStream<T> baseStream) {
    return new MultiTraversalStream<>(baseStream);
  }

  public MultiTraversalStream<T> newIndependentStream() {
    return new MultiTraversalStream<>(baseStream, streamNode);
  }

  public Optional<T> take() {
    streamNode = streamNode.getNext();
    return streamNode.get();
  }

  public boolean isClosed() {
    return baseStream.isClosed();
  }

  private static class StreamNode<T> {
    private LazyValue<StreamNode<T>> next;
    private T value;

    public StreamNode(@Nullable T value, LazyValue<StreamNode<T>> next) {
      this.value = value;
      this.next = next;
    }

    public Optional<T> get() {
      return Optional.fromNullable(value);
    }

    public StreamNode<T> getNext() {
      return next.get();
    }
  }

  private class StreamNodeLazyValue extends LazyValue<StreamNode<T>> {
    @Override
    protected StreamNode<T> compute() {
      return new StreamNode<T>(baseStream.take().orNull(), new StreamNodeLazyValue());
    }
  }
}
