import("oscillator.lib");

freq = hslider("[1]frequency", 440, 0, 20000, 0.0001);
amp = hslider("[2]amp", 1, 0, 1, 0.00001);
width = hslider("[3]width", 0.5, 0, 1, 0.0000);

process = pulsetrainN(2, freq, width) * amp;
