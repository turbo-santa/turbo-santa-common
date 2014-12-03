noinst_LIBRARIES = libclocktroller.a

libclocktroller_a_SOURCES = clocktroller.h clocktroller.cc clocktroller_test.cc clocktroller_test.h

check_PROGRAMS = clocktroller_test
AM_DEFAULT_SOURCE_EXT = .cc
clocktroller_test_LDFLAGS = -pthread
clocktroller_test_LDADD = libclocktroller.a ../opcode_executor/libopcode_executor.a ../memory/libmemory.a ${top_srcdir}/test_harness/libtest_harness.a ${top_srcdir}/third_party/gtest/lib/libgtest_main.a ${top_srcdir}/third_party/gtest/lib/libgtest.a ../graphics/graphics_controller.o

TESTS = ${check_PROGRAMS}
