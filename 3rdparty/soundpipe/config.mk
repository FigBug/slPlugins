# Modules that don't require external libraries go here
MODULES= \
base \
ftbl \
tevent \
adsr \
allpass \
atone \
autowah \
bal \
bar \
biquad \
biscale \
blsaw \
blsquare \
bltriangle \
fold \
bitcrush \
butbp \
butbr \
buthp \
butlp \
clip \
comb \
compressor \
count \
conv \
crossfade \
dcblock \
delay \
dist \
dmetro \
drip \
dtrig \
dust \
eqfil \
expon \
fof \
fog \
fofilt \
foo \
fosc \
gbuzz \
hilbert \
in \
incr \
jcrev \
jitter \
line \
lpf18 \
maygate \
metro \
mincer \
mode \
moogladder \
noise \
nsmp \
osc \
oscmorph \
pan2 \
panst \
pareq \
paulstretch \
pdhalf \
peaklim \
phaser \
phasor \
pinknoise \
pitchamdf \
pluck \
port \
posc3 \
progress \
prop \
pshift \
ptrack \
randh \
randi \
randmt \
random \
reverse \
reson \
revsc \
rms \
rpt \
samphold \
scale \
scrambler \
sdelay \
slice \
smoothdelay \
streson \
switch \
tabread \
tadsr \
tblrec \
tbvcf \
tdiv \
tenv \
tenv2 \
tenvx \
tgate \
thresh \
timer \
tin \
tone \
trand \
tseg \
tseq \
vdelay \
vocoder \
waveset \
wpkorg35 \
zitarev

ifndef NO_LIBSNDFILE
	MODULES += diskin
else
	CFLAGS += -DNO_LIBSNDFILE
endif

# ini parser needed for nsmp module
include lib/inih/Makefile

# Header files needed for modules generated with FAUST
CFLAGS += -Ilib/faust

# fft library
include lib/fft/Makefile

# JACK module
#
#MODULES += jack
#CFLAGS += -ljack

# RPi Module
#
#MODULES += rpi
#CFLAGS += -lasound

include lib/kissfft/Makefile
MODULES += fftwrapper
MODULES += padsynth

# Uncomment to use FFTW3 instead of kissfft.
# CFLAGS += -lfftw3 -DUSE_FFTW3

# Soundpipe audio 
# Soundpipe audio 
#include lib/spa/Makefile
#CFLAGS += -DUSE_SPA

CFLAGS += -fPIC -g
