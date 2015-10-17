java_library(
  name = "streams",
  srcs = [
    "InStream.java",
    "OutStream.java",
    "InStreamManipulators.java",
    "StringPrinterSinkProcess.java",
  ],
  deps = ["//external:guava"],
  visibility = ["//visibility:public"],
)

java_library(
  name = "stream_runner",
  srcs = [
    "StreamRunner.java",
    "StreamRunnerBuilder.java",
  ],
  deps = [
    "//java/com/turbosanta/backend/logging:consumer",
    "//java/com/turbosanta/backend/logging:consumer_builder",
    "//protobuf/backend/logging:message_java",
    ":streams",
  ],
  visibility = ["//visibility:public"],
)

java_library(
  name = "multi_traversal_stream",
  srcs = ["MultiTraversalStream.java"],
  deps = [
    "//external:findbugs",
    "//external:guava",
    ":lazy_value",
    ":streams",
  ],
  visibility = ["//visibility:public"],
)

java_library(
  name = "lazy_value",
  srcs = ["LazyValue.java"],
  visibility = ["//visibility:public"],
)
