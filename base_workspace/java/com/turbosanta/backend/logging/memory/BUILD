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
