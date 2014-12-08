noinst_LTLIBRARIES = libclocktroller.la

libclocktroller_la_SOURCES = clocktroller.h clocktroller.cc

check_PROGRAMS = clocktroller_test
AM_DEFAULT_SOURCE_EXT = .cc
clocktroller_test_SOURCES = clocktroller_test.cc clocktroller_test.h
clocktroller_test_LDFLAGS = -pthread
clocktroller_test_LDADD = libclocktroller.la ${top_srcdir}/back_end/opcode_executor/libopcode_executor.la ${top_srcdir}/back_end/memory/libmemory.la ${top_srcdir}/test_harness/libtest_harness.la ${top_srcdir}/back_end/graphics/libgraphics.la ${top_srcdir}/third_party/gtest/lib/libgtest_main.a ${top_srcdir}/third_party/gtest/lib/libgtest.a ${top_srcdir}/back_end/debugger/libdebugger.la

TESTS = ${check_PROGRAMS}
