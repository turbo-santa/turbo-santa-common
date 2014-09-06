include ../Makefile.inc
CPPFLAGS= $(PROJFLAGS) -I ..

SOURCES= opcode_handlers.cc opcode_parser.cc
OBJS= $(SOURCES:.cc=.o)

all: $(SOURCES) $(OBJS)

opcode_handlers.o: opcode_handlers.cc opcode_handlers.h registers.h

opcode_parser.o: opcode_parser.cc registers.h opcodes.h
