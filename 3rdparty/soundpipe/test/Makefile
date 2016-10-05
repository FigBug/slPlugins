.PHONY: clean perf

all: libsptest.a run.bin perf

config.mk: config.def.mk
	cp config.def.mk config.mk

include config.mk

OBJ = $(addprefix t/, $(addsuffix .o, $(TESTS)))
PERF_OBJ = $(addprefix p/, $(PERF))

LDFLAGS += -L/usr/local/lib -lsndfile -lm
CFLAGS += -g -I../h -I /usr/local/include -I. -O3 -Wall -Werror

ifdef WRITE_RAW
CFLAGS += -DWRITE_RAW
endif

libsptest.a: md5.o test.o libtap.o
	ld -o $@ -r md5.o libtap.o test.o

t/%.o: t/%.c
	$(CC) -c $(CFLAGS) -o $@ $<

perf: $(PERF_OBJ)
p/p_%: p/p_%.c
	$(CC) $< $(CFLAGS) -lsoundpipe -lsndfile -lm -o $@

perftest: 
	./perf_test $(PERF_OBJ)

plot:
	gnuplot plot.plt
	convert -rotate 90 demo.png out.png

test.o: test.c
	$(CC) -g -I../h -I /usr/local/include -c -o $@ $<

run.bin: run.c test.c md5.c libsptest.a $(OBJ) all_tests.h
	$(CC) run.c -Wall $(CFLAGS) $(LDFLAGS) -o$@ $(OBJ) ../libsoundpipe.a libsptest.a -lm -lsndfile

clean:
	rm -rf run.bin libsptest.a *.o $(OBJ) *.raw $(PERF_OBJ) *.png *.log
