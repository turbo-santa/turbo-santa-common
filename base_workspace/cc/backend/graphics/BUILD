cc_library(
  name = "graphics_controller",
  hdrs = ["graphics_controller.h"],
  srcs = ["graphics_controller.cc"],
  deps = [
    "//cc/backend/memory:interrupt_flag",
    "//cc/backend/memory:module",
    "//cc/backend/memory:primary_flags",
    "//cc/backend/memory:vram_segment",
    ":graphics_flags",
    ":screen",
  ],
  visibility = ["//visibility:public"],
)

cc_library(
  name = "screen",
  hdrs = ["screen.h"],
  deps = ["//submodules:glog"],
  visibility = ["//visibility:public"],
)

cc_library(
  name = "graphics_flags",
  hdrs = ["graphics_flags.h"],
  deps = ["//submodules:glog"],
  visibility = ["//visibility:public"],
)

cc_library(
  name = "dma_transfer",
  hdrs = ["dma_transfer.h"],
  srcs = ["dma_transfer.cc"],
  deps = [
    "//cc/backend/memory:memory_mapper",
    "//submodules:glog",
  ],
  visibility = ["//visibility:public"],
)
