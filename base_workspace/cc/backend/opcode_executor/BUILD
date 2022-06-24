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
    "//cc/backend/memory:memory_mapper",
    ":registers",
  ],
)

cc_library(
  name = "opcode_parser",
  hdrs = ["opcode_parser.h"],
  srcs = ["opcode_parser.cc"],
  deps = [
    "//cc/backend/decompiler:decompiler",
    "//cc/backend/decompiler:decompiler_factory",
    "//cc/backend/memory:memory_layout",
    "//cc/backend/memory:memory_mapper_rom_bridge",
    "@com_github_google_glog//:glog",
  ],
)

cc_library(
  name = "opcode_executor",
  hdrs = ["opcode_executor.h"],
  srcs = ["opcode_executor.cc"],
  deps = [
    "//cc/backend/decompiler:decompiler",
    "//cc/backend/decompiler:decompiler_factory",
    "//cc/backend/memory/interrupt:interrupt_flag",
    "//cc/backend/memory/interrupt:primary_flags",
    "//cc/backend/memory:memory_mapper",
    "//cc/backend/memory:memory_mapper_rom_bridge",
    "@com_github_google_glog//:glog",
    ":executor_context",
    ":opcode_map",
    ":opcode_parser",
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
    "//cc/backend/decompiler:instruction",
    "//cc/backend/memory:memory_mapper",
    "@com_github_google_glog//:glog",
    ":executor_context",
    ":opcodes",
    ":registers",
  ],
)

# disabled becaues test_harness is failing
#cc_test(
#  name = "opcode_handlers_test",
#  srcs = ["opcode_handlers_test.cc", "opcode_executor.cc", "opcode_executor.h"],
#  deps = [
#    "//cc/backend/graphics:graphics_controller",
#    "//cc/backend/graphics:screen",
#    "//cc/backend/memory/dma_transfer",
#    "//cc/backend/memory/interrupt:primary_flags",
#    "//cc/backend/memory/mbc:mbc_module",
#    "//cc/backend/memory/ram:default_module",
#    "//cc/backend/memory/unimplemented:unimplemented_module",
#    "//cc/backend/memory:memory_mapper",
#    "//cc/backend/memory:memory_mapper_rom_bridge",
#    "//cc/backend/memory:memory_layout",
#    # "//cc/test_harness", # FAILING C++
#    "@com_github_google_glog//:glog",
#    "@com_google_googletest//:gtest_main",
#    ":opcode_map",
#    ":opcode_parser",
#  ],
#)
