# failing C++ code

#     cc/test_harness/test_harness.cc: In member function 'void test_harness::TestHarness::SetMemoryState(const test_harness::MemoryAddressValuePair&)':
#     cc/test_harness/test_harness.cc:44:57: error: request for member 'get' in '((test_harness::TestHarness*)this)->test_harness::TestHarness::parser_->backend::opcode_executor::OpcodeExecutor::memory_mapper_', which is of pointer type 'backend::memory::MemoryMapper*' (maybe you meant to use '->' ?)
#        44 |   MemoryMapper* memory_mapper = parser_->memory_mapper_.get();
#           |                                                         ^~~
#     cc/test_harness/test_harness.cc: In member function 'testing::AssertionResult test_harness::TestHarness::AssertMemoryState(const std::vector<test_harness::MemoryAddressValuePair>&)':
#     cc/test_harness/test_harness.cc:67:59: error: request for member 'get' in '((test_harness::TestHarness*)this)->test_harness::TestHarness::parser_->backend::opcode_executor::OpcodeExecutor::memory_mapper_', which is of pointer type 'backend::memory::MemoryMapper*' (maybe you meant to use '->' ?)
#        67 |     MemoryMapper* memory_mapper = parser_->memory_mapper_.get();
#           |                                                           ^~~


#cc_library(
#  name = "test_harness",
#  hdrs = [
#    "test_harness.h",
#    "test_harness_utils.h",
#  ],
#  srcs = ["test_harness.cc"],
#  deps = [
#    "//cc/backend/opcode_executor",
#    "@com_google_googletest//:gtest_main",
#  ],
#  visibility = ["//visibility:public"],
#)
