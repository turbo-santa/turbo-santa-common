noinst_LTLIBRARIES = libopcode_executor.la

libopcode_executor_la_SOURCES = opcode_executor.h opcode_executor.cc opcode_handlers.h opcode_handlers.cc opcode_map.h opcode_map.cc opcodes.h opcodes.cc registers.h

check_PROGRAMS = opcode_handlers_test
AM_DEFAULT_SOURCE_EXT = .cc
opcode_handlers_test_SOURCES = opcode_handlers_test.cc
opcode_handlers_test_LDFLAGS = -pthread
opcode_handlers_test_LDADD = libopcode_executor.la ${top_srcdir}/back_end/memory/libmemory.la ${top_srcdir}/test_harness/libtest_harness.la ${top_srcdir}/third_party/gtest/lib/libgtest_main.a ${top_srcdir}/third_party/gtest/lib/libgtest.a ${top_srcdir}/back_end/graphics/libgraphics.la ${top_srcdir}/back_end/clocktroller/libclocktroller.la ${top_srcdir}/back_end/debugger/libdebugger.la

TESTS = ${check_PROGRAMS}
