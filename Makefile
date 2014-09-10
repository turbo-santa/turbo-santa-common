include Makefile.inc

TESTEXE= test
CPPFLAGS= $(PROJFLAGS) -I .
OBJLIBS= libbackend.a libmemory.a
LIBS= -L. -lbackend -lmemory #-L/usr/lib/x86_64-linux-gnu -l:libstdc++.so.6 -l:/lib/x86_64-linux-gnu/libc.so.6 -l:/lib/x86_64-linux-gnu/libgcc_s.so.1
OBJS= test_harness.o main.o

all: $(TESTEXE)

$(TESTEXE): $(OBJLIBS) $(OBJS)
	$(CC) -o $(TESTEXE) $(OBJS) $(LIBS)

libbackend.a libmemory.a: force_look
	$(MAKE) -C back_end

test_harness.o main.o: force_look
	$(MAKE) -C test_harness

clean:
	rm -rf $(OBJS) $(OBJLIBS) $(TESTEXE)
	$(MAKE) -C back_end clean
	$(MAKE) -C test_harness clean

force_look:
	true

.PHONY: clean force_look
