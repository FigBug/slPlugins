import("oscillator.lib");
amp = hslider("amp", 1, 0, 1, 0.0001);
process = pink_noise * amp;
