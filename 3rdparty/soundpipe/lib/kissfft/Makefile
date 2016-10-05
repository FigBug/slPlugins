LPATHS += $(LIBDIR)/kissfft/kiss_fft.o $(LIBDIR)/kissfft/kiss_fftr.o
CFLAGS += -Ilib/kissfft/ -Dkiss_fft_scalar=float

$(LIBDIR)/kissfft:
	mkdir -p $@

$(LIBDIR)/kissfft/kiss_fft.o: lib/kissfft/kiss_fft.c | $(LIBDIR)/kissfft
	$(CC) $< -c $(CFLAGS) -o $@

$(LIBDIR)/kissfft/kiss_fftr.o: lib/kissfft/kiss_fftr.c | $(LIBDIR)/kissfft
	$(CC) $< -c $(CFLAGS) -o $@
