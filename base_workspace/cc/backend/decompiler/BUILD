cc_library(
  name = "instruction",
  hdrs = ["instruction.h"],
  visibility = ["//visibility:public"],
)

cc_library(
  name = "raw_instruction",
  hdrs = ["raw_instruction.h"],
  srcs = ["raw_instruction.cc"],
  deps = ["//external:glog"],
)

cc_library(
  name = "instruction_factory",
  hdrs = ["instruction_factory.h"],
  deps = [
    ":instruction",
    ":raw_instruction",
  ],
)

cc_library(
  name = "instruction_factory_helpers",
  hdrs = ["instruction_factory_helpers.h"],
  srcs = ["instruction_factory_helpers.cc"],
  deps = [
    ":instruction",
    ":instruction_factory",
    ":raw_instruction",
  ],
)

cc_library(
  name = "instruction_map",
  hdrs = ["instruction_map.h"],
  srcs = ["instruction_map.cc"],
  deps = [
    "//external:glog",
    ":instruction",
    ":instruction_factory",
    ":instruction_factory_helpers",
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
    "//external:glog",
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
    "//external:glog",
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
    "//external:glog",
    ":decompiler",
    ":decompiler_factory",
    ":rom_bridge",
  ],
)
