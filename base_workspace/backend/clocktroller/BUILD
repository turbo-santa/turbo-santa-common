cc_library(
  name = "clocktroller",
  hdrs = ["clocktroller.h"],
  srcs = [
    ":clocktroller.cc",
  ],
  deps = [
    "//backend:config",
    "//backend/opcode_executor",
    "//backend/opcode_executor:opcodes",
    "//backend/opcode_executor:registers",
  ],
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
