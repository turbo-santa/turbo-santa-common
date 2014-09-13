include Makefile.inc

TESTEXE= test
CPPFLAGS= $(PROJFLAGS) -I .
OBJLIBS= libbackend.a libmemory.a libtestharness.a
LIBS= -L. -lbackend -lmemory -ltestharness
OBJS=

all: $(OBJLIBS) $(OBJS)

libbackend.a libmemory.a: force_look
	$(MAKE) -C back_end

libtestharness.a: force_look
	$(MAKE) -C test_harness

clean:
	rm -rf $(OBJS) $(OBJLIBS) $(TESTEXE)
	$(MAKE) -C back_end clean
	$(MAKE) -C test_harness clean

force_look:
	true

.PHONY: clean force_look
