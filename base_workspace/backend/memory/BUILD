cc_library(
  name = "memory_segment",
  hdrs = ["memory_segment.h"],
)

cc_library(
  name = "memory_layout",
  hdrs = ["memory_layout.h"],
  visibility = ["//visibility:public"],
)

cc_library(
  name = "ram_segment",
  hdrs = ["ram_segment.h"],
  deps = [":memory_segment"],
)

cc_library(
  name = "vram_segment",
  hdrs = ["vram_segment.h"],
  deps = [
    "//submodules:glog",
    ":memory_segment",
  ],
  visibility = ["//backend/graphics:__pkg__"]
)

cc_library(
  name = "echo_segment",
  hdrs = ["echo_segment.h"],
  deps = [
    "//submodules:glog",
    ":memory_segment",
    ":ram_segment",
  ],
)

cc_library(
  name = "flags",
  hdrs = ["flags.h"],
  deps = [":memory_segment"],
)

cc_library(
  name = "internal_rom",
  hdrs = ["internal_rom.h"],
  deps = [
    "//submodules:glog",
    ":flags",
    ":memory_segment",
  ],
)

cc_library(
  name = "interrupt_flag",
  hdrs = ["interrupt_flag.h"],
  deps = [
    "//submodules:glog",
    ":memory_segment",
  ],
  visibility = ["//visibility:public"],
)

cc_library(
  name = "mbc",
  hdrs = ["mbc.h"],
  srcs = ["mbc.cc"],
  deps = [
    "//submodules:glog",
    ":memory_segment",
  ],
)

cc_library(
  name = "default_module",
  hdrs = ["default_module.h"],
  deps = [
    ":echo_segment",
    ":memory_segment",
    ":module",
    ":ram_segment",
  ],
  visibility = ["//visibility:public"],
)

cc_library(
  name = "dma_transfer",
  hdrs = ["dma_transfer.h"],
  deps = [
    ":flags",
    ":memory_mapper",
    ":module",
  ],
  visibility = ["//visibility:public"],
)

cc_library(
  name = "joypad_module",
  hdrs = ["joypad_module.h"],
  deps = [
    "//submodules:glog",
    ":flags",
    ":interrupt_flag",
    ":module",
  ],
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
  name = "mbc_module",
  hdrs = ["mbc_module.h"],
  deps = [
    ":internal_rom",
    ":mbc",
    ":memory_segment",
    ":module",
  ],
  visibility = ["//visibility:public"],
)

cc_library(
  name = "unimplemented_module",
  hdrs = ["unimplemented_module.h"],
  deps = [
    ":flags",
    ":module",
  ],
  visibility = ["//visibility:public"],
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
  name = "primary_flags",
  hdrs = ["primary_flags.h"],
  deps = [
    "//submodules:glog",
    ":interrupt_flag",
    ":module",
  ],
  visibility = ["//visibility:public"],
)

cc_library(
  name = "memory_mapper_rom_bridge",
  hdrs = ["memory_mapper_rom_bridge.h"],
  deps = [
    "//backend/decompiler:rom_bridge",
    ":mbc",
    ":memory_mapper",
  ],
  visibility = ["//visibility:public"],
)

cc_library(
  name = "memory_mapper",
  hdrs = ["memory_mapper.h"],
  srcs = ["memory_mapper.cc"],
  deps = [
    "//submodules:glog",
    ":flags",
    ":flag_container",
    ":memory_segment",
    ":module",
  ],
  visibility = ["//visibility:public"],
)
