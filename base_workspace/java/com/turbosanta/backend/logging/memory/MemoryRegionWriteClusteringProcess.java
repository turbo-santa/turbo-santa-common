package com.turbosanta.backend.logging.memory;

import com.google.common.base.Optional;
import com.google.common.collect.ImmutableList;
import com.turbosanta.backend.logging.streams.InStream;
import com.turbosanta.backend.logging.streams.InStreamManipulators.Process;


public class MemoryRegionWriteClusteringProcess
    implements Process<MemoryRegionWriteEvent, MemoryRegionWriteCluster> {
  private final int clusterRadius;

  private ImmutableList.Builder<MemoryRegionWriteEvent> currentCluster;

  public MemoryRegionWriteClusteringProcess(int clusterRadius) {
    this.clusterRadius = clusterRadius;

    currentCluster = ImmutableList.<MemoryRegionWriteEvent>builder();
  }

  @Override
  public InStream<MemoryRegionWriteCluster> apply(InStream<MemoryRegionWriteEvent> inputStream) {
    return inputStream
      .flatMap((MemoryRegionWriteEvent writeEvent) -> cluster(writeEvent));
  }

  private Optional<MemoryRegionWriteCluster> cluster(MemoryRegionWriteEvent writeEvent) {
    if (writeEvent.getDistanceToPreviousMemoryRegionWriteEvent() < clusterRadius) {
      currentCluster.add(writeEvent);
      return Optional.absent();
    } else {
      MemoryRegionWriteCluster cluster = new MemoryRegionWriteCluster(currentCluster.build());
      currentCluster = ImmutableList.builder();
      currentCluster.add(writeEvent);
      return Optional.of(cluster);
    }
  }
}
