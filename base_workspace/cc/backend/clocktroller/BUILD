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

# C++ code is failing
#
#       cc/test_harness/test_harness.cc: In member function 'void test_harness::TestHarness::SetMemoryState(const test_harness::MemoryAddressValuePair&)':
#       cc/test_harness/test_harness.cc:44:57: error: request for member 'get' in '((test_harness::TestHarness*)this)->test_harness::TestHarness::parser_->backend::opcode_executor::OpcodeExecutor::memory_mapper_', which is of pointer type 'backend::memory::MemoryMapper*' (maybe you meant to use '->' ?)
#          44 |   MemoryMapper* memory_mapper = parser_->memory_mapper_.get();
#             |                                                         ^~~
#       cc/test_harness/test_harness.cc: In member function 'testing::AssertionResult test_harness::TestHarness::AssertMemoryState(const std::vector<test_harness::MemoryAddressValuePair>&)':
#       cc/test_harness/test_harness.cc:67:59: error: request for member 'get' in '((test_harness::TestHarness*)this)->test_harness::TestHarness::parser_->backend::opcode_executor::OpcodeExecutor::memory_mapper_', which is of pointer type 'backend::memory::MemoryMapper*' (maybe you meant to use '->' ?)
#          67 |     MemoryMapper* memory_mapper = parser_->memory_mapper_.get();
#             |                                  

#cc_test(
#  name = "clocktroller_test",
#  srcs = [
#    "clocktroller_test.h",
#    "clocktroller_test.cc",
#  ],
#  deps = [
#    "//cc/backend/opcode_executor:registers",
#    "@com_github_google_glog//:glog",
#    "@com_google_googletest//:gtest_main",
#    "//cc/test_harness",
#    ":clocktroller"
#  ],
#)
