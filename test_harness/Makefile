include ../Makefile.inc
CPPFLAGS= $(PROJFLAGS) -I ..

# TODO(Brendan): Update the names.
SOURCES= test_harness.cc main.cc
OBJS= ../test_harness.o ../main.o

all: $(OBJS)

../test_harness.o: test_harness.cc
	$(CC) $(CPPFLAGS) -c test_harness.cc -o ../test_harness.o

../main.o: main.cc
	$(CC) $(CPPFLAGS) -c main.cc -o ../main.o

clean:
	rm -rf $(OBJS)

.PHONY: clean
