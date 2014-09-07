include ../Makefile.inc
CPPFLAGS= $(PROJFLAGS) -I ../..

SOURCES= memory_mapper.cc 
OBJS= $(SOURCES:.cc=.o)

all: $(SOURCES) $(OBJS)

memory_mapper.o: memory_mapper.cc memory_mapper.h memory_page.h memory_segment.h
