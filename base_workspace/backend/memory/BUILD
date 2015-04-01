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
  visibility = ["//backend/graphics:__pkg__"]
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
  visibility = ["//visibility:public"],
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
  name = "default_module",
  hdrs = ["default_module.h"],
  deps = [
    "//backend:config",
    ":echo_segment",
    ":memory_segment",
    ":module",
    ":ram_segment",
  ],
  visibility = ["//visibility:public"],
)

cc_library(
  name = "flag_container",
  hdrs = ["flag_container.h"],
  deps = [
    "//backend:config",
    ":flags",
    ":memory_segment",
  ],
)

cc_library(
  name = "mbc_module",
  hdrs = ["mbc_module.h"],
  deps = [
    "//backend:config",
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
    "//backend:config",
    ":flags",
    ":module",
  ],
  visibility = ["//visibility:public"],
)

cc_library(
  name = "module",
  hdrs = ["module.h"],
  deps = [
    "//backend:config",
    ":memory_segment",
    ":flags",
  ],
  visibility = ["//visibility:public"],
)

cc_library(
  name = "primary_flags",
  hdrs = ["primary_flags.h"],
  deps = [
    "//backend:config",
    "//submodules:glog",
    ":interrupt_flag",
    ":module",
  ],
  visibility = ["//visibility:public"],
)

cc_library(
  name = "memory_mapper",
  hdrs = ["memory_mapper.h"],
  srcs = ["memory_mapper.cc"],
  deps = [
    "//backend:config",
    "//submodules:glog",
    ":flags",
    ":flag_container",
    ":memory_segment",
    ":module",
  ],
  visibility = ["//visibility:public"],
)
