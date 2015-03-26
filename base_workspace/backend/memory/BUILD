cc_library(
  name = "memory_segment",
  hdrs = ["memory_segment.h"],
  deps = ["//backend:config"],
)

cc_library(
  name = "ram_segment",
  hdrs = ["ram_segment.h"],
  deps = [
    "//backend:config",
    ":memory_segment",
  ],
)

cc_library(
  name = "vram_segment",
  hdrs = ["vram_segment.h"],
  deps = [
    "//backend:config",
    "//submodules:glog",
    ":memory_segment",
  ],
)

cc_library(
  name = "echo_segment",
  hdrs = ["echo_segment.h"],
  deps = [
    "//backend:config",
    "//submodules:glog",
    ":memory_segment",
    ":ram_segment",
  ],
)

cc_library(
  name = "flags",
  hdrs = ["flags.h"],
  deps = [
    "//backend:config",
    ":memory_segment",
  ],
)

cc_library(
  name = "internal_rom",
  hdrs = ["internal_rom.h"],
  deps = [
    "//backend:config",
    "//submodules:glog",
    ":flags",
    ":memory_segment",
  ],
)

cc_library(
  name = "interrupt_flag",
  hdrs = ["interrupt_flag.h"],
  deps = [
    "//backend:config",
    "//submodules:glog",
    ":memory_segment",
  ],
)

cc_library(
  name = "mbc",
  hdrs = ["mbc.h"],
  srcs = ["mbc.cc"],
  deps = [
    "//backend:config",
    "//submodules:glog",
    ":memory_segment",
  ],
)

cc_library(
  name = "memory_mapper",
  hdrs = ["memory_mapper.h"],
  srcs = ["memory_mapper.cc"],
  deps = [
    "//backend:config",
    "//backend/graphics:dma_transfer",
    "//backend/graphics:graphics_flags",
    "//submodules:glog",
    ":echo_segment",
    ":internal_rom",
    ":interrupt_flag",
    ":mbc",
    ":memory_segment",
    ":ram_segment",
    ":vram_segment",
  ],
  visibility = ["//visibility:public"],
)
