include ../../Makefile.inc
CPPFLAGS= $(PROJFLAGS) -I ../..

SOURCES= memory_mapper.cc memory_segment.cc
OBJS= $(SOURCES:.cc=.o)
OBJLIBS= ../../libmemory.a

all: $(OBJLIBS)

../../libmemory.a: $(OBJS)
	$(AR) rv ../../libmemory.a $(OBJS)

$(OBJS): $(SOURCES)

clean:
	rm -rf $(OBJS) $(OBJLIBS)

.PHONY: clean
