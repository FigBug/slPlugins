# What on Earth is a crossfeed filter?

Headphones have extreme stereo separation--the right ear doesn't get to hear
much of what's going on on the left. This leads to the impression the music's
coming from inside your head, and sounds especially weird when instruments are
panned hard to one side or the other.

Crossfeed filters aim to fix this by letting the channels mix a little, but in
a controlled way. The goal is to mimic what happens naturally when listening
to music on speakers.

# Why do we need yet another crossfeed filter?

I'd tried using existing crossfeed filters, but _hated_ the sound. Besides the
loss of high frequencies, they just sound weird to me. The theory seemed
sound, but the execution left something to be desired.

I decided to try something weird--to design a filter that could change the
stereo image without impacting overall frequency response. This filter does
that two ways:

* Monaural signals pass through unchanged. The filter converts to mid/side
  stereo, works on the side channel, then converts back to the regular
  left/right format.
* For e.g. a sound in the left channel, the filter modifies the left channel
  signal in a way that complements the sound introduced on the right. If you
  were to run this filter, then mix the output down to a single channel, these
  two effects would cancel each other out--the result is the same as mixing
  the original down to mono! (Minus any rounding noise introduced.)

If the goal is to change nothing but stereo imaging, this sounds like an
excellent sign!

# How can I try this?

If you have a recent version of Mac OS X (in theory 10.7 and up should work,
though I've only tested on 10.9), there's a command-line player included.
Thanks to Apple's audio frameworks, it gains support for a decent range of
audio formats.

This player is basically the crudest thing that let me test it myself--it may
contain bugs and it's definitely missing features. Even so, I've spent
countless hours listening to music through it, so it's probably good enough to
decide if you like the filter's effect!

An example:

    $ ./crossfeed-player -g -6 /.ITunes\ Music/Ingrid\ Michaelson/Be\ OK/01\ Be\ OK.m4a

This starts playing "Be OK" (on my system, where my iTunes Music folder is
in a non-standard place) with the volume reduced 6dB.

I recommend letting it reduce volume by at least a couple decibels to prevent
clipping.

A crazier example:

    $ find . -type f -not -name '.DS_Store' -not -name '._*' -print0 | xargs -0o ~/crossfeed-player/crossfeed-player -s -g -8

This shuffles and plays everything in the current working directory (assuming
xargs doesn't have to call the player multiple times). It might try to play
album art and such too, but it won't succeed--it'll refuse and go to the next
file.

While the program's running, it responds to a few commands:
* q: Quit
* <: Previous song
* >: Next song
* c: Toggle crossfeed filter on/off, in case you're not sure what you're
  supposed to be hearing
* /: Decrease volume
* *: Increase volume (These make more sense if you have a number pad)

If you do try this, please let me know what you think of it!
