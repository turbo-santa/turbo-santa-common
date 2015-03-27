cc_library(
  name = "graphics_controller",
  hdrs = ["graphics_controller.h"],
  srcs = ["graphics_controller.cc"],
  deps = [
    "//backend:config",
    "//backend/memory:interrupt_flag",
    "//backend/memory:module",
    "//backend/memory:primary_flags",
    "//backend/memory:vram_segment",
    ":graphics_flags",
    ":screen",
  ],
  visibility = ["//visibility:public"],
)

cc_library(
  name = "screen",
  hdrs = ["screen.h"],
  deps = [
    "//backend:config",
    "//submodules:glog",
  ],
  visibility = ["//visibility:public"],
)

cc_library(
  name = "graphics_flags",
  hdrs = ["graphics_flags.h"],
  deps = [
    "//backend:config",
    "//submodules:glog",
  ],
  visibility = ["//visibility:public"],
)

cc_library(
  name = "dma_transfer",
  hdrs = ["dma_transfer.h"],
  srcs = ["dma_transfer.cc"],
  deps = [
    "//backend:config",
    "//backend/memory:memory_mapper",
    "//submodules:glog",
  ],
  visibility = ["//visibility:public"],
)
