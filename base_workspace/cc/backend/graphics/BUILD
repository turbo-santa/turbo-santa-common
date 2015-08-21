cc_library(
  name = "graphics_controller",
  hdrs = ["graphics_controller.h"],
  srcs = ["graphics_controller.cc"],
  deps = [
    "//cc/backend/memory/interrupt:interrupt_flag",
    "//cc/backend/memory/interrupt:primary_flags",
    "//cc/backend/memory:module",
    ":graphics_flags",
    ":screen",
    ":vram_segment",
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
  name = "vram_segment",
  hdrs = ["vram_segment.h"],
  deps = [
    "//cc/backend/memory:memory_segment",
    "//submodules:glog",
  ],
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
