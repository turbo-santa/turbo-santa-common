cc_library(
  name = "interrupt_flag",
  hdrs = ["interrupt_flag.h"],
  deps = [
    "//cc/backend/memory:memory_segment",
    "//submodules:glog",
  ],
  visibility = ["//visibility:public"],
)

cc_library(
  name = "primary_flags",
  hdrs = ["primary_flags.h"],
  deps = [
    "//cc/backend/memory:module",
    "//submodules:glog",
    ":interrupt_flag",
  ],
  visibility = ["//visibility:public"],
)

