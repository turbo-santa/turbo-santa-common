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
  name = "memory_profiler_consumer_factory",
  srcs = ["MemoryProfilerConsumerFactory.java"],
  deps = [
    "//java/com/turbosanta/backend/logging:abstract_consumer_factory",
    "//java/com/turbosanta/backend/logging/streams",
    "//protobuf/backend/logging:message_java",
    ":memory_profiler_consumer",
  ],
  visibility = ["//visibility:public"],
)
