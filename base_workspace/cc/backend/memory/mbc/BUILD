cc_library(
  name = "mbc_module",
  hdrs = ["mbc_module.h"],
  deps = [
    "//cc/backend/memory:memory_segment",
    "//cc/backend/memory:module",
    ":internal_rom",
    ":mbc",
  ],
  visibility = ["//visibility:public"],
)

cc_library(
  name = "mbc",
  hdrs = ["mbc.h"],
  srcs = ["mbc.cc"],
  deps = [
    "//cc/backend/memory:memory_segment",
    "//external:glog",
  ],
  visibility = ["//cc/backend/memory:__pkg__"],
)

cc_library(
  name = "internal_rom",
  hdrs = ["internal_rom.h"],
  deps = [
    "//cc/backend/memory:flags",
    "//cc/backend/memory:memory_segment",
    "//external:glog",
  ],
)

