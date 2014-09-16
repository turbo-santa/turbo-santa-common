include ../../Makefile.inc
PROJECT_DIR= ../..
CPPFLAGS= $(PROJFLAGS) -I ../..

SOURCES= memory_mapper.cc memory_segment.cc
OBJS= $(SOURCES:.cc=.o)
OBJLIBS= $(PROJECT_DIR)/libmemory.a

all: $(OBJLIBS)

$(PROJECT_DIR)/libmemory.a: $(OBJS)
	$(AR) rv $(PROJECT_DIR)/libmemory.a $(OBJS)

$(OBJS): $(SOURCES)

clean:
	rm -rf $(OBJS) $(OBJLIBS)

.PHONY: clean
