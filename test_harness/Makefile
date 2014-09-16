include ../Makefile.inc
PROJECT_DIR= ..
CPPFLAGS= $(PROJFLAGS) -I ..

# TODO(Brendan): Update the names.
SOURCES= test_harness.cc main.cc
OBJS= $(PROJECT_DIR)/test_harness.o ../main.o
OBJLIBS= $(PROJECT_DIR)/libtestharness.a

all: $(OBJS) $(OBJLIBS)

$(PROJECT_DIR)/test_harness.o: test_harness.cc
	$(CC) $(CPPFLAGS) -c test_harness.cc -o $(PROJECT_DIR)/test_harness.o

$(PROJECT_DIR)/main.o: main.cc
	$(CC) $(CPPFLAGS) -c main.cc -o $(PROJECT_DIR)/main.o

$(PROJECT_DIR)/libtestharness.a: $(PROJECT_DIR)/test_harness.o
	$(AR) rv $(PROJECT_DIR)/libtestharness.a $(PROJECT_DIR)/test_harness.o

clean:
	rm -rf $(OBJS) $(OBJLIBS)

.PHONY: clean
