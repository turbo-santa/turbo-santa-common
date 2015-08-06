cc_library(
  name = "clocktroller",
  hdrs = ["clocktroller.h"],
  srcs = [
    ":clocktroller.cc",
  ],
  deps = [
    "//cc/backend/graphics:graphics_controller",
    "//cc/backend/graphics:screen",
    "//cc/backend/memory:default_module",
    "//cc/backend/memory:dma_transfer",
    "//cc/backend/memory:joypad_module",
    "//cc/backend/memory/timer",
    "//cc/backend/memory:mbc_module",
    "//cc/backend/memory:memory_mapper",
    "//cc/backend/memory:primary_flags",
    "//cc/backend/memory:unimplemented_module",
    "//cc/backend/opcode_executor",
  ],
  visibility = ["//visibility:public"],
)

cc_test(
  name = "clocktroller_test",
  srcs = [
    "clocktroller_test.h",
    "clocktroller_test.cc",
  ],
  deps = [
    "//cc/backend/opcode_executor:registers",
    "//submodules:glog",
    "//submodules:googletest",
    "//test_harness",
  ],
)
