include ../Makefile.inc
CPPFLAGS= $(PROJFLAGS) -I ..

SOURCES= opcode_handlers.cc opcode_map.cc registers.cc opcodes.cc opcode_parser.cc
OBJS= $(SOURCES:.cc=.o)
OBJLIBS= ../libbackend.a ../libmemory.a

all: $(OBJLIBS)

../libbackend.a: $(OBJS)
	$(AR) rv ../libbackend.a $(OBJS)

../libmemory.a: force_look
	$(MAKE) -C memory

opcode_handlers.o: opcode_handlers.cc opcode_handlers.h registers.h

clean:
	rm -rf $(OBJS) $(OBJLIBS)
	$(MAKE) -C memory clean

force_look:
	true

.PHONY: clean force_look
