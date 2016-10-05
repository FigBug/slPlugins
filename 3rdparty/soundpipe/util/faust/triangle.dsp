import("oscillator.lib");

freq = hslider("[0]freq", 440, 0, 20000, 0.0001);
amp = hslider("[1]amp", 1, 0, 1, 0.00001);

process = triangle(freq) * amp;
