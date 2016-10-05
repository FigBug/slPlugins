import("effect.lib");

atk = hslider("[0] atk", 0.01, 0.0001, 0.5, 0.00001);
rel = hslider("[1] rel", 0.01, 0.0001, 0.5, 0.00001);
bwratio = hslider("[2] bwratio", 0.5, 0.1, 2, 0.001);
process = vocoder(16, atk, rel, bwratio);
