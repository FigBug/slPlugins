.PHONY: default clean all

default: all

config.mk: config.def.mk
	cp config.def.mk config.mk

include config.mk


all: config.mk $(addsuffix .bin, $(files)) 

%.bin : %.c 
	gcc -g --std=c99 -I ../h -L /usr/local/lib -I /usr/local/include $< ../libsoundpipe.a -lsndfile -o $@ -lm

clean:
	rm  -rf $(addsuffix .bin, $(files)) $(addsuffix .bin.dSYM, $(files))  test.wav plot.dat plot.png
