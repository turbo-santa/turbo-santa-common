SUBDIRS = memory opcode_executor clocktroller graphics debugger

noinst_LTLIBRARIES = libback_end.la

libback_end_la_SOURCES =

libback_end_la_LIBADD = memory/libmemory.la opcode_executor/libopcode_executor.la clocktroller/libclocktroller.la graphics/libgraphics.la

bin_PROGRAMS = turbo_test

turbo_test_SOURCES = main.cc

turbo_test_LDADD = libback_end.la
