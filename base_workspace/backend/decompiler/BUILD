cc_library(
  name = "instruction",
  hdrs = ["instruction.h"],
  visibility = ["//visibility:public"],
)

cc_library(
  name = "raw_instruction",
  hdrs = ["raw_instruction.h"],
  srcs = ["raw_instruction.cc"],
  deps = ["//submodules:glog"],
)

cc_library(
  name = "instruction_factory",
  hdrs = ["instruction_factory.h"],
  srcs = ["instruction_factory.cc"],
  deps = [
    ":instruction",
    ":raw_instruction",
  ],
)

cc_library(
  name = "instruction_map",
  hdrs = ["instruction_map.h"],
  srcs = ["instruction_map.cc"],
  deps = [
    "//submodules:glog",
    ":instruction",
    ":instruction_factory",
    ":raw_instruction",
  ],
)

cc_library(
  name = "rom_bridge",
  hdrs = ["rom_bridge.h"],
  visibility = ["//visibility:public"],
)

cc_library(
  name = "rom_reader",
  hdrs = ["rom_reader.h"],
  srcs = ["rom_reader.cc"],
  deps = [
    "//submodules:glog",
    ":instruction",
    ":instruction_factory",
    ":instruction_map",
    ":raw_instruction",
    ":rom_bridge",
  ],
  visibility = ["//visibility:public"],
)

cc_library(
  name = "decompiler",
  hdrs = ["decompiler.h"],
  srcs = ["decompiler.cc"],
  deps = [
    "//submodules:glog",
    ":instruction",
    ":instruction_map",
    ":rom_bridge",
    ":rom_reader",
  ],
  visibility = ["//visibility:public"],
)

cc_library(
  name = "decompiler_factory",
  hdrs = ["decompiler_factory.h"],
  deps = [
    ":decompiler",
    ":rom_bridge",
  ],
  visibility = ["//visibility:public"],
)

cc_binary(
  name = "decompile",
  srcs = ["main.cc"],
  deps = [
    "//submodules:glog",
    ":decompiler",
    ":decompiler_factory",
    ":rom_bridge",
  ],
)
