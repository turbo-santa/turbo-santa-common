java_library(
  name = "memory_profiler_consumer",
  srcs = ["MemoryProfilerConsumer.java"],
  deps = [
    "//external:guava",
    "//java/com/turbosanta/backend/logging:consumer",
    "//java/com/turbosanta/backend/logging/streams",
    "//protobuf/backend/logging:message_java",
  ],
  visibility = ["//visibility:public"],
)

java_library(
  name = "memory_profiler_consumer_builder",
  srcs = ["MemoryProfilerConsumerBuilder.java"],
  deps = [
    "//java/com/turbosanta/backend/logging:consumer_builder",
    "//java/com/turbosanta/backend/logging/streams",
    "//protobuf/backend/logging:message_java",
    ":memory_profiler_consumer",
  ],
  visibility = ["//visibility:public"],
)

java_library(
  name = "memory_region_write_event_printing_pipeline",
  srcs = ["MemoryRegionWriteEventPrintingPipeline.java"],
  deps = [
    "//java/com/turbosanta/backend/logging/streams",
    "//protobuf/backend/logging:message_java",
    ":memory_event_context_filtering_process",
    ":memory_region_write_event",
    ":memory_region_write_event_filtering_process",
  ],
  visibility = ["//visibility:public"],
)

java_library(
  name = "memory_event_context_filtering_process",
  srcs = ["MemoryEventContextFilteringProcess.java"],
  deps = [
    "//external:guava",
    "//java/com/turbosanta/backend/logging/streams",
    "//protobuf/backend/logging:message_java",
    ":memory_event",
    ":memory_event_context",
  ],
)

java_library(
  name = "memory_region_write_event_filtering_process",
  srcs = ["MemoryRegionWriteEventFilteringProcess.java"],
  deps = [
    "//external:guava",
    "//java/com/turbosanta/backend/logging/streams",
    ":memory_event",
    ":memory_event_context",
    ":memory_region_write_event",
  ],
)

java_library(
  name = "memory_region_write_event",
  srcs = ["MemoryRegionWriteEvent.java"],
  deps = [
    ":memory_event",
    ":memory_event_context",
  ],
)

java_library(
  name = "memory_event_context",
  srcs = ["MemoryEventContext.java"],
  deps = [":memory_event"],
)

java_library(
  name = "memory_event",
  srcs = ["MemoryEvent.java"],
  deps = [
    "//external:guava",
    "//protobuf/backend/logging:message_java",
  ],
)
