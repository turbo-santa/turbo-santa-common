cc_library(
  name = "clocktroller",
  hdrs = ["clocktroller.h"],
  srcs = [
    ":clocktroller.cc",
  ],
  deps = [
    "//backend:config",
    "//backend/graphics:graphics_controller",
    "//backend/graphics:screen",
    "//backend/memory:default_module",
    "//backend/memory:mbc_module",
    "//backend/memory:memory_mapper",
    "//backend/memory:primary_flags",
    "//backend/opcode_executor",
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
    "//backend:config",
    "//backend/opcode_executor:registers",
    "//submodules:glog",
    "//submodules:googletest",
    "//test_harness",
  ],
)
