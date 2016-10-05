LPATHS += $(LIBDIR)/inih/ini.o
CFLAGS += -Ilib/inih/

$(LIBDIR)/inih:
	mkdir -p $@

$(LIBDIR)/inih/ini.o: lib/inih/ini.c | $(LIBDIR)/inih
	$(CC) $(CFLAGS) $< -c -o $@
