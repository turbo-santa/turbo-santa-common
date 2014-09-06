include Makefile.inc

CFLAGS= $(PROJFLAGS) -I .

back_end_dir:
	$(MAKE) -C back_end
