cc_library(
  name = "default_module",
  hdrs = ["default_module.h"],
  deps = [
    "//cc/backend/memory:memory_segment",
    "//cc/backend/memory:module",
    ":echo_segment",
    ":ram_segment",
  ],
  visibility = ["//visibility:public"],
)

cc_library(
  name = "ram_segment",
  hdrs = ["ram_segment.h"],
  deps = ["//cc/backend/memory:memory_segment"],
)

cc_library(
  name = "echo_segment",
  hdrs = ["echo_segment.h"],
  deps = [
    "//cc/backend/memory:memory_segment",
    "//submodules:glog",
    ":ram_segment",
  ],
)

