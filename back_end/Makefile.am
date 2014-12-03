SUBDIRS = memory opcode_executor clocktroller graphics

noinst_LIBRARIES = libback_end.a

libback_end_a_SOURCES =

libback_end_a_LIBADD = memory/libmemory.a opcode_executor/libopcode_executor.a clocktroller/libclocktroller.a graphics/libgraphics.a

bin_PROGRAMS = turbo_test

turbo_test_SOURCES = main.cc

turbo_test_LDADD = memory/*.o ./opcode_executor/opcode_handlers.o ./opcode_executor/opcode_map.o opcode_executor/libopcode_executor.a opcode_executor/opcode_executor.o ./clocktroller/clocktroller.o graphics/*.o
