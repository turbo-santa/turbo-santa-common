include ../Makefile.inc
CPPFLAGS= $(PROJFLAGS) -I ..

SOURCES= test_harness.cc
OBJS= ../test_harness.o

all: $(OBJS)

../test_harness.o: test_harness.cc
	$(CC) $(CPPFLAGS) -c test_harness.cc -o ../test_harness.o

clean:
	rm -rf $(OBJS)

.PHONY: clean
