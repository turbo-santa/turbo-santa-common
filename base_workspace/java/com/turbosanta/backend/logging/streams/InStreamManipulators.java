package com.turbosanta.backend.logging.streams;

import com.google.common.base.Optional;
import java.util.Collection;
import java.util.Collections;

public class InStreamManipulators {
  private InStreamManipulators() {}

  @FunctionalInterface
  public static interface Process<I, O> {
    InStream<O> apply(InStream<I> inputStream);
  }

  @FunctionalInterface
  public static interface MapFunction<I, O> {
    O apply(I input);
  }

  @FunctionalInterface
  public static interface FlatMapFunctionWithCollection<I, O> extends MapFunction<I, Collection<O>> {
    @Override
    Collection<O> apply(I input);
  }

  @FunctionalInterface
  public static interface FlatMapFunctionWithOptional<I, O> extends MapFunction<I, Optional<O>> {
    @Override
    Optional<O> apply(I input);
  }

  @FunctionalInterface
  public static interface Sink<T> {
    void apply(T input);
  }

  @FunctionalInterface
  public static interface FilterFunction<T> {
    boolean apply(T input);
  }

  public static class OptionalToCollectionApplicator<I, O> implements FlatMapFunctionWithCollection<I, O> {
    private FlatMapFunctionWithOptional<I, O> wrappedFunction;

    public OptionalToCollectionApplicator(FlatMapFunctionWithOptional<I, O> wrappedFunction) {
      this.wrappedFunction = wrappedFunction;
    }

    @Override
    public Collection<O> apply(I input) {
      Optional<O> result = wrappedFunction.apply(input);
      if (result.isPresent()) {
        return Collections.singleton(result.get());
      } else {
        return Collections.emptyList();
      }
    }
  }

  public static class SinkApplicator<T> implements FlatMapFunctionWithOptional<T, Void> {
    private Sink<T> sinkFunction;

    public SinkApplicator(Sink<T> sinkFunction) {
      this.sinkFunction = sinkFunction;
    }

    @Override
    public Optional<Void> apply(T input) {
      sinkFunction.apply(input);
      return Optional.absent();
    }
  }

  public static class FilterApplicator<T> implements FlatMapFunctionWithOptional<T, T> {
    private FilterFunction<T> filterFunction;

    public FilterApplicator(FilterFunction<T> filterFunction) {
      this.filterFunction = filterFunction;
    }

    @Override
    public Optional<T> apply(T input) {
      if (filterFunction.apply(input)) {
        return Optional.of(input);
      } else {
        return Optional.absent();
      }
    }
  }
}
