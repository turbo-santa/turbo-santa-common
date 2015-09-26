cc_library(
  name = "memory",
  hdrs = ["memory.h"],
  srcs = ["memory.cc"],
  deps = [
    "//cc/backend/graphics:graphics_controller",
    "//cc/backend/graphics:screen",
    "//cc/backend/memory/ram:default_module",
    "//cc/backend/memory/dma_transfer",
    "//cc/backend/memory/joypad:joypad_module",
    "//cc/backend/memory/timer",
    "//cc/backend/memory/mbc:mbc_module",
    "//cc/backend/memory:memory_mapper",
    "//cc/backend/memory/interrupt:primary_flags",
    "//cc/backend/memory/unimplemented:unimplemented_module",
  ],
  visibility = ["//visibility:public"],
)

cc_library(
  name = "memory_segment",
  hdrs = ["memory_segment.h"],
  visibility = ["//visibility:public"],
)

cc_library(
  name = "memory_layout",
  hdrs = ["memory_layout.h"],
  visibility = ["//visibility:public"],
)

cc_library(
  name = "flags",
  hdrs = ["flags.h"],
  deps = [":memory_segment"],
  visibility = ["//visibility:public"],
)

cc_library(
  name = "flag_container",
  hdrs = ["flag_container.h"],
  deps = [
    ":flags",
    ":memory_segment",
  ],
)

cc_library(
  name = "module",
  hdrs = ["module.h"],
  deps = [
    ":memory_segment",
    ":flags",
  ],
  visibility = ["//visibility:public"],
)

cc_library(
  name = "memory_mapper_rom_bridge",
  hdrs = ["memory_mapper_rom_bridge.h"],
  deps = [
    "//cc/backend/decompiler:rom_bridge",
    "//cc/backend/memory/mbc",
    ":memory_mapper",
  ],
  visibility = ["//visibility:public"],
)

cc_library(
  name = "memory_mapper",
  hdrs = ["memory_mapper.h"],
  srcs = ["memory_mapper.cc"],
  deps = [
    "//cc/backend/logging",
    "//external:glog",
    ":flags",
    ":flag_container",
    ":memory_segment",
    ":module",
  ],
  visibility = ["//visibility:public"],
)
