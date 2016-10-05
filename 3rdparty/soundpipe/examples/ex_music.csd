<CsoundSynthesizer>
<CsOptions>
;-d -odac:system:playback_ -+rtaudio="jack" 
-odac -rtaudio+=alsa
;--sample-accurate
-Z
-B 4096
-b 4096
</CsOptions>
<CsInstruments>

sr	=	22050
ksmps	=	1024
nchnls	=	1
0dbfs	=	1

giwav ftgen 0, 0, 2048, 1, "FMSine111.wav", 0, 0, 0
gisine ftgen 0, 0, 4096, 10, 1
gaRev init 0
seed 0
instr 1	

a1 oscili 0.3, cpsmidinn(p4), giwav
arand randomi 0, 1, 0.8, 0, 0
a1 *= arand
gaRev += a1
endin


instr 999
aL, aR reverbsc gaRev, gaRev, 0.97, 10000
aL dcblock aL
out aL * 0.3 + gaRev * 0.5
gaRev = 0;
endin

</CsInstruments>
<CsScore>

i999 0 40
i1 0 40 60
i1 0 40 67
i1 0 40 71
i1 0 40 74
i1 0 40 76


</CsScore>
</CsoundSynthesizer>

