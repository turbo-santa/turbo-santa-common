java_library(
  name = "streams",
  srcs = [
    "InStream.java",
    "OutStream.java",
  ],
  deps = ["//external:guava"],
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
