include Makefile.inc
PROJECT_DIR= .

TESTEXE= test
CPPFLAGS= $(PROJFLAGS) -I .
OBJLIBS= libbackend.a libmemory.a libtestharness.a
LIBS= -L. -lbackend -lmemory -ltestharness
OBJS=

all: $(OBJLIBS) $(OBJS)

libbackend.a: force_look
	$(MAKE) -C back_end/opcode_executor

libmemory.a: force_look
	$(MAKE) -C back_end/memory

libtestharness.a: force_look
	$(MAKE) -C test_harness

clean:
	rm -rf $(OBJS) $(OBJLIBS) $(TESTEXE)
	$(MAKE) -C back_end/opcode_executor clean
	$(MAKE) -C back_end/memory clean
	$(MAKE) -C test_harness clean

force_look:
	true

.PHONY: clean force_look
