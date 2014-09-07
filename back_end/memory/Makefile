include ../../Makefile.inc
CPPFLAGS= $(PROJFLAGS) -I ../..

SOURCES= memory_mapper.cc 
OBJS= $(SOURCES:.cc=.o)
OBJLIBS= ../../libmemory.a

all: $(OBJLIBS)

../../libmemory.a: $(OBJS)
	$(AR) rv ../../libmemory.a $(OBJS)

memory_mapper.o: memory_mapper.cc memory_mapper.h memory_page.h memory_segment.h

clean:
	rm -rf $(OBJS) $(OBJLIBS)

.PHONY: clean
