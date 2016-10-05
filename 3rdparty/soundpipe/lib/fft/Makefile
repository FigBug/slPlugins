LPATHS += $(LIBDIR)/fft/fft.o
UTIL += $(LIBDIR)/fft/fft.c

$(LIBDIR)/fft:
	mkdir -p $@

$(LIBDIR)/fft/fft.c: lib/fft/fftlib.c lib/fft/sp_fft.c | $(LIBDIR)/fft
	echo "/* THIS IS A GENERATED FILE. DO NOT EDIT BY HAND */" > $@
	cat $^ >> $@

$(LIBDIR)/fft/fft.o: $(LIBDIR)/fft/fft.c | $(LIBDIR)/fft
	$(CC) $< -c $(CFLAGS) -o $@
