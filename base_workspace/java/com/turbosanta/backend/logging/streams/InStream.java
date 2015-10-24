package com.turbosanta.backend.logging.streams;

import java.util.Queue;

import com.google.common.base.Optional;
import com.google.common.collect.Lists;
import com.turbosanta.backend.logging.streams.InStreamManipulators.MapFunction;
import com.turbosanta.backend.logging.streams.InStreamManipulators.OptionalToCollectionApplicator;
import com.turbosanta.backend.logging.streams.InStreamManipulators.Process;
import com.turbosanta.backend.logging.streams.InStreamManipulators.Sink;
import com.turbosanta.backend.logging.streams.InStreamManipulators.SinkApplicator;
import com.turbosanta.backend.logging.streams.InStreamManipulators.FilterFunction;
import com.turbosanta.backend.logging.streams.InStreamManipulators.FlatMapFunctionWithCollection;
import com.turbosanta.backend.logging.streams.InStreamManipulators.FlatMapFunctionWithOptional;
import com.turbosanta.backend.logging.streams.InStreamManipulators.FilterApplicator;

public abstract class InStream<T> {
  public abstract Optional<T> take();
  public abstract boolean isClosed();

  public <S> InStream<S> applyProcess(Process<T, S> process) {
    return process.apply(this);
  }

  public <S> InStream<S> map(final MapFunction<T, S> mapFunction) {
    return new InStream<S>() {

      @Override
      public Optional<S> take() {
        Optional<T> value = InStream.this.take();
        if (value.isPresent()) {
          return Optional.of(mapFunction.apply(value.get()));
        } else {
          return Optional.absent();
        }
      }

      @Override
      public boolean isClosed() {
        return InStream.this.isClosed();
      }
    };
  }

  public <S> InStream<S> flatMap(final FlatMapFunctionWithCollection<T, S> mapFunction) {
    return new InStream<S>() {
      private Queue<Optional<S>> buffer = Lists.newLinkedList();

      @Override
      public Optional<S> take() {
        while (buffer.isEmpty()) {
          refillBuffer();
        }
        return buffer.remove();
      }

      private void refillBuffer() {
        Optional<T> value = InStream.this.take();
        if (value.isPresent()) {
          for (S elem : mapFunction.apply(value.get())) {
            buffer.add(Optional.of(elem));
          }
        } else {
          buffer.add(Optional.<S>absent());
        }
      }

      @Override
      public boolean isClosed() {
        return InStream.this.isClosed();
      }
    };
  }

  public <S> InStream<S> flatMap(FlatMapFunctionWithOptional<T, S> mapFunction) {
    return flatMap(new OptionalToCollectionApplicator<T, S>(mapFunction));
  }

  public InStream<Void> forEach(Sink<T> sinkFunction) {
    return flatMap(new SinkApplicator<>(sinkFunction));
  }

  public InStream<T> filter(final FilterFunction<T> filterFunction) {
    return flatMap(new FilterApplicator<T>(filterFunction));
  }
}
