include ../Makefile.inc
CPPFLAGS= $(PROJFLAGS) -I ..

DIRS= memory
SOURCES= opcode_handlers.cc opcode_parser.cc
OBJS= $(SOURCES:.cc=.o)

all: $(SOURCES) $(OBJS) $(DIRS)

opcode_handlers.o: opcode_handlers.cc opcode_handlers.h registers.h

opcode_parser.o: opcode_parser.cc registers.h opcodes.h

memory:
	$(MAKE) -C memory
