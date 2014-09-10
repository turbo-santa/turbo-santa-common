include ../Makefile.inc
CPPFLAGS= $(PROJFLAGS) #-I ..

SOURCES= opcode_handlers.cc opcode_map.cc registers.cc opcodes.cc opcode_parser.cc
OBJS= $(SOURCES:.cc=.o)
OBJLIBS= $(PROJECT_DIR)/libbackend.a $(PROJECT_DIR)/libmemory.a
GTEST_OBJLIB= $(PROJECT_DIR)/third_party/gtest/gtest_main.a
TEST_SOURCES= opcode_handlers_test.cc
TEST_OBJS= $(TEST_SOURCES:.cc=.o)
TESTS= test_bin

all: $(OBJLIBS)

$(PROJECT_DIR)/libbackend.a: $(OBJS)
	$(AR) rv $(PROJECT_DIR)/libbackend.a $(OBJS)

$(PROJECT_DIR)/libmemory.a: force_look
	$(MAKE) -C memory

test: $(TESTS)
	./test_bin

$(TESTS): $(TEST_OBJS) $(OBJS) $(GTEST_OBJLIB) $(PROJECT_DIR)/libmemory.a $(PROJECT_DIR)/libtestharness.a
	$(CC) -o $(TESTS) $(TEST_OBJS) $(OBJS) -l:$(PROJECT_DIR)/third_party/gtest/gtest_main.a -l:$(PROJECT_DIR)/libmemory.a -lpthread -l:$(PROJECT_DIR)/libtestharness.a

$(GTEST_OBJLIB): force_look
	$(MAKE) -C $(PROJECT_DIR)/third_party/gtest/

$(PROJECT_DIR)/libtestharness.a: force_look
	$(MAKE) -C $(PROJECT_DIR)/test_harness

clean:
	rm -rf $(OBJS) $(OBJLIBS) $(TEST_OBJS) $(TESTS)
	$(MAKE) -C memory clean

force_look:
	true

.PHONY: clean force_look
