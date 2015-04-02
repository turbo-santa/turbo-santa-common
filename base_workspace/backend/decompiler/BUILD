cc_library(
  name = "instruction",
  hdrs = ["instruction.h"],
  deps = ["//backend:config"],
  visibility = ["//visibility:public"],
)

cc_library(
  name = "raw_instruction",
  hdrs = ["raw_instruction.h"],
  deps = [
    "//backend:config",
    "//submodules:glog",
  ],
)

cc_library(
  name = "instruction_factory",
  hdrs = ["instruction_factory.h"],
  deps = [
    "//backend:config",
    ":instruction",
    ":raw_instruction",
  ],
)

cc_library(
  name = "instruction_map",
  hdrs = ["instruction_map.h"],
  srcs = ["instruction_map.cc"],
  deps = [
    "//backend:config",
    "//submodules:glog",
    ":instruction",
    ":instruction_factory",
    ":raw_instruction",
  ],
)

cc_library(
  name = "rom_reader",
  hdrs = ["rom_reader.h"],
  srcs = ["rom_reader.cc"],
  deps = [
    "//backend:config",
    "//submodules:glog",
    ":instruction",
    ":instruction_factory",
    ":instruction_map",
    ":raw_instruction",
  ],
  visibility = ["//visibility:public"],
)
