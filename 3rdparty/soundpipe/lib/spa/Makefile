LPATHS += $(LIBDIR)/spa/spa.o
CFLAGS += -Ilib/spa/
HPATHS += lib/spa/spa.h

$(LIBDIR)/spa:
	mkdir -p $@

$(LIBDIR)/spa/spa.o: lib/spa/spa.c | $(LIBDIR)/spa
	gcc $(CFLAGS) $< -c -o $@
