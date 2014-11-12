noinst_LIBRARIES = libopcode_executor.a

libopcode_executor_a_SOURCES = opcode_executor.h opcode_executor.cc opcode_handlers.h opcode_handlers.cc opcode_map.h opcode_map.cc opcodes.h opcodes.cc registers.h registers.cc

check_PROGRAMS = opcode_handlers_test
AM_DEFAULT_SOURCE_EXT = .cc
opcode_handlers_test_SOURCES = opcode_executor.cc opcode_handlers_test.cc
opcode_handlers_test_LDFLAGS = -pthread
opcode_handlers_test_LDADD =  libopcode_executor.a ../memory/libmemory.a ${top_srcdir}/test_harness/libtest_harness.a ${top_srcdir}/third_party/gtest/lib/libgtest_main.a ${top_srcdir}/third_party/gtest/lib/libgtest.a

TESTS = ${check_PROGRAMS}
