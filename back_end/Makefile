include ../Makefile.inc
CPPFLAGS= $(PROJFLAGS) -I ..

DIRS= memory
SOURCES= opcode_handlers.cc
OBJS= $(SOURCES:.cc=.o)

all: $(SOURCES) $(OBJS) $(DIRS)

.PHONY:
clean:
	rm -rf $(OBJS)

opcode_handlers.o: opcode_handlers.cc opcode_handlers.h registers.h

opcode_parser.o: opcode_parser.h registers.h opcodes.h

memory:
	$(MAKE) -C memory
