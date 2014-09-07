include Makefile.inc

TESTEXE= test
CPPFLAGS= $(PROJFLAGS) -I .
OBJLIBS= libbackend.a libmemory.a
LIBS= -L. -lbackend -lmemory -L/usr/lib -lstdc++
OBJS= test_harness.o

all: $(TESTEXE)

$(TESTEXE): $(OBJLIBS) $(OBJS)
	$(LD) -o $(TESTEXE) $(OBJS) $(LIBS)

libbackend.a libmemory.a: force_look
	$(MAKE) -C back_end

test_harness.o: force_look
	$(MAKE) -C test_harness

clean:
	rm -rf $(OBJS) $(OBJLIBS) $(TESTEXE)
	$(MAKE) -C back_end clean
	$(MAKE) -C test_harness clean

force_look:
	true

.PHONY: clean force_look
