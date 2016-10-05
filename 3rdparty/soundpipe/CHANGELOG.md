# Change log

1.4.0
-----
    - added smoothdelay module
    - changed padsynth gen routine behavior to play nicer with oscillator
    - randh: divide by zero fixes
    - prop: added {}, |, and ? operators
    - added gen_rand
    - debugging symbol flag (-g) added to config.mk instead of Makefile
    - added slice ugen
    - fixed wpkorg35 bug when cutoff is set to 1000  and res to 1.0
    - prop: added sp_prop_reset
    - added scrambler gen routine
    - header file more robust
    - nsmp valgrind fixes
    - added performance testing utility (initial steps)
 
1.3.0
-----
    - added tseg module
    - added wpkorg35 module
    - removed complex.h dependency
    - added "help" option to test CLI
    - lots and lots of tests added
    - added "waveset" module
    - adsr bugfix: retriggering is possible when sustain is 0
    - added ptrack module
    - (re)-added reson module
    - added pdhalf module
    - removed strok_r dependency
    - rewrote sp_gen_vals
    - tabread tweaks for audiokit
    - added peaklim module

1.2.0
-----
    - added internal soundpipe file (SPA)
    - removed sp->k
    - paulstretch now loops
    - added tdiv ugen
    - Completly removed setbuf and getbuf helper functions
    - added hilbert transform

1.1.1
-----
    - Fixed clicking in static delay (simplified code)
    - Testing improvements:
        - test utility now has command line args:
            - "./run.bin" works like it did before
            - "./run.bin gen_header" rewrites all_tests.h 
            - "./run.bin regen_header" rewrites all_tests.h, but regenerates md5 hashes
            - "./run.bin test id" runs a single test
            - "./run.bin render id" renders a single test
    - Tests added:
        - adsr
    - Added module 'tblrec' 
    - Added module 'paulstretch'
    - Fixes to tabread: 'mode' is now mandatory value in the init function
    - dist: removed mode (internal code change)
    - clip: simplified immensly. Now only does method 1 from Csound (sine method)
    - padsynth: fixed a malloc issue that was causing valgrind to complain
    - Fixes to rpt: no longer freaks out if repeat sizes are out of bounds
    - Replaced pink noise algorithm from faust code to Protrekkr code

1.01 
----
    - New modules:
        - fof 
        - fog
        - compressor
        - pshift: pitch shifter
        - timer
        - adsr
        - tadsr
        - crossfade
        - oscmorph
        - vocoder
    - New ftable gen routine: gen_composite (needed for fof/fog)

1.0: AudioKit
-------------
    - New Modules:
        - oscmorph
        - adsr
        - tadsr
    - ftable bug fix
    - tseq fixes
    - line/expon are now triggerable

0.6 
--------------
    - New Modules:
        - conv: performs partitioned convolution
        - pitchamdf: pitch tracking using average magnitude difference function
        - bandlimited oscillators: saw, square, triangle
        - autowah
        - pink noise generator
        - phaser
        - thresh
        - trand: triggerable RNG
    - pluck rewritten and simplified
    - removed "event" module
    - make examples more consistent
    - prop: added [] to double duration
    - nsmp: reads from ftbl instead of disk
    - tabread rewritten to do linear interpolation
    - attribution for zitarev and tweaks
    - Makefile/config.mk reorganization
    - Improvements towards Faust automation

0.5 "Spectral"
--------------
    - Internal RNG created for cross-platform consistency
    - NO_LIBSOUNDFILE compilation flag
    - config.mk can be a variable for multiple config files
    - kissfft library codebase added
    - gen_padsynth uses kissfft instead of fftw (and is now a default module)
    - FFTs for RISC by John Green added to Soundpipe codebase
    - improved testing interface 
    - Many tests (50+) created
    - Dynamic library generation enabled by default (in addition to static lib)
    - New Modules:
        - mincer
        - line
        - expon
        - tenvx 
        - zitarev
     - New gen routine: load_soundfile
     - Fixes:
        - Removed AAIO dependency in sp_jack
        - fixed interpolation bug with fosc, posc3, and osc
        - better comments in boilerplate code
        - removed seed argument in sp_randi_init
        - osx makefile fixes
        - minor fixes to docgen output

0.4 
---
    - License change: GPL to MIT
    - sp_progress CLI progress bar
    - New modules: scale, butbr, pan2, pareq, tenv2, jcrev, barmodel
    - Modules can now be made with Faust!
    - Testing utility and framework (libsptest) created. See test folder
    - Improved docgen
    - Bug fixes

0.3 "Automation"
----------------
    - New modules:
        - eqfil
        - atone
        - butbr
        - foo (dummy module)
        - pluck
        - delay (static delay with feedback)
        - random
        - diskin
        - in
        - nsmp
        - switch
        - dmetro
        - samphold
    - Module boostrapper with documentation
    - renamed delay to vdelay
    - Documented gen routines
    - Style guide written

0.2.2: "Tables"
---------------
    - New gen routines: gen_line, gen_xline, gen_gauss
    - New Modules: vco, reson, allpass, phasor, comb, tabread
    - Docgen support for gen routines
    - A standardized layout for gen routines
    - Bug fixes relating to docgen, maxdel, and libsndfile valgrind errors
    - Musical example 5 (byte beats!)

0.2.1: "Mo Modules"
-------------------
    - New modules: dist, gbuzz fosc, butbp, drip, clip,
    balance, rms, jitter, lpf18
    - Updated: mode, jack, rpi, tenv
    - 2 new musical examples
    - updated html docgen to work with Lynx better
    - amalgamation of soundpipe sources possible
    - sp_param/sp_set
0.2.0
-----
    - html documentation generator
    - metadata for current modules created in modules/data 
    - improved build instructions for OSX
    - example cleanup
    - module variable cleanup
    - Port of Csound GEN07 (gen_line)
    - Multi-channel and Multi-mono file support
    - Revsc module is now stereo
    - This changelog exists

0.1.0
-----

    initial release
