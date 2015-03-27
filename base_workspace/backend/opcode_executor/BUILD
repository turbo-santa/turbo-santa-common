cc_library(
  name = "registers",
  hdrs = ["registers.h"],
  deps = ["//backend:config"],
  visibility = ["//visibility:public"],
)

cc_library(
  name = "opcodes",
  hdrs = ["opcodes.h"],
  srcs = ["opcodes.cc"],
  deps = ["//backend:config"],
  visibility = ["//visibility:public"],
)

cc_library(
  name = "opcode_executor",
  hdrs = ["opcode_executor.h"],
  srcs = ["opcode_executor.cc"],
  deps = [
    "//backend/memory:interrupt_flag",
    "//backend/memory:memory_mapper",
    "//backend/memory:primary_flags",
    "//backend:config",
    "//submodules:glog",
    ":opcode_map",
    ":opcodes",
    ":registers",
  ],
  visibility = ["//visibility:public"],
)

cc_library(
  name = "opcode_map",
  hdrs = [
    "opcode_map.h",
    "opcode_handlers.h",
  ],
  srcs = [
    "opcode_map.cc",
    "opcode_handlers.cc",
  ],
  deps = [
    "//backend:config",
    "//backend/memory:memory_mapper",
    "//submodules:glog",
    ":opcodes",
    ":registers",
  ],
)

cc_test(
  name = "opcode_handlers_test",
  srcs = ["opcode_handlers_test.cc"],
  deps = [
    "//backend:config",
    "//submodules:glog",
    "//test_harness",
    ":opcode_map",
  ],
)
