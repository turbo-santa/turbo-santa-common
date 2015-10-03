cc_library(
  name = "clocktroller",
  hdrs = ["clocktroller.h"],
  srcs = [
    ":clocktroller.cc",
  ],
  deps = [
    "//cc/backend/graphics:screen",
    "//cc/backend/memory",
    "//cc/backend/memory/interrupt:primary_flags",
    "//cc/backend/memory/joypad:joypad_module",
    "//cc/backend/memory:memory_mapper",
    "//cc/backend/opcode_executor",
    "//external:glog",
  ],
  linkopts = ["-pthread"],
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
    "//external:glog",
    "//external:gtest",
    "//test_harness",
  ],
)
