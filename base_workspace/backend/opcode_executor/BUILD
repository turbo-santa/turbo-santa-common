cc_library(
  name = "registers",
  hdrs = ["registers.h"],
  visibility = ["//visibility:public"],
)

cc_library(
  name = "opcodes",
  hdrs = ["opcodes.h"],
  srcs = ["opcodes.cc"],
  visibility = ["//visibility:public"],
)

cc_library(
  name = "executor_context",
  hdrs = ["executor_context.h"],
  deps = [
    "//backend/memory:memory_mapper",
    ":registers",
  ],
)

cc_library(
  name = "opcode_executor",
  hdrs = ["opcode_executor.h"],
  srcs = ["opcode_executor.cc"],
  deps = [
    "//backend/decompiler:decompiler",
    "//backend/decompiler:decompiler_factory",
    "//backend/memory:interrupt_flag",
    "//backend/memory:memory_mapper",
    "//backend/memory:memory_mapper_rom_bridge",
    "//backend/memory:primary_flags",
    "//submodules:glog",
    ":executor_context",
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
    "//backend/decompiler:instruction",
    "//backend/memory:memory_mapper",
    "//submodules:glog",
    ":executor_context",
    ":opcodes",
    ":registers",
  ],
)

cc_test(
  name = "opcode_handlers_test",
  srcs = ["opcode_handlers_test.cc"],
  deps = [
    "//submodules:glog",
    "//test_harness",
    ":opcode_map",
  ],
)
