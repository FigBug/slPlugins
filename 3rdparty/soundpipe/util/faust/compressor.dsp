import("effect.lib");

ratio = hslider("[0] ratio", 1, 1, 40, 0.001);
thresh = hslider("[1] thresh", 0, -80, 0, 0.001);
atk = hslider("[2] atk", 0.1, 0, 10, 0.001);
rel = hslider("[3] rel", 0.1, 0, 10, 0.001);

process = compressor_mono(ratio, thresh, atk, rel);
