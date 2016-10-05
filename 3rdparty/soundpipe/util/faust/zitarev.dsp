import("effect.lib");

my_zita(x,y) = zita_rev1_stereo(rdel,f1,f2,t60dc,t60m,fsmax,x,y)
	  : out_eq : dry_wet(x,y) : out_level
with {

  fsmax = 96000.0;  // highest sampling rate that will be used


  rdel = hslider("[00] in_delay [unit:ms] [style:knob]
                  [tooltip: Delay in ms before reverberation begins]",
                  60,10,100,1);


  f1 = hslider("[01] lf_x [unit:Hz] [style:knob] [scale:log]
       [tooltip: Crossover frequency (Hz) separating low and middle frequencies]",
       200, 50, 1000, 1);

  t60dc = hslider("[02] rt60_low [unit:s] [style:knob] [scale:log]
          [style:knob] [tooltip: T60 = time (in seconds) to decay 60dB in low-frequency band]",
	  3, 1, 8, 0.1);

  t60m = hslider("[03] rt60_mid [unit:s] [style:knob] [scale:log]
          [tooltip: T60 = time (in seconds) to decay 60dB in middle band]",
	  2, 1, 8, 0.1);

  f2 = hslider("[04] hf_damping [unit:Hz] [style:knob] [scale:log]
       [tooltip: Frequency (Hz) at which the high-frequency T60 is half the middle-band's T60]",
       6000, 1500, 0.49*fsmax, 1);

  out_eq = pareq_stereo(eq1f,eq1l,eq1q) : pareq_stereo(eq2f,eq2l,eq2q);
// Zolzer style peaking eq (not used in zita-rev1) (filter.lib):
// pareq_stereo(eqf,eql,Q) = peak_eq(eql,eqf,eqf/Q), peak_eq(eql,eqf,eqf/Q);
// Regalia-Mitra peaking eq with "Q" hard-wired near sqrt(g)/2 (filter.lib):
  pareq_stereo(eqf,eql,Q) = peak_eq_rm(eql,eqf,tpbt), peak_eq_rm(eql,eqf,tpbt)
  with {
    tpbt = wcT/sqrt(max(0,g)); // tan(PI*B/SR), B bw in Hz (Q^2 ~ g/4)
    wcT = 2*PI*eqf/SR;  // peak frequency in rad/sample
    g = db2linear(eql); // peak gain
  };

  eq1f = hslider("[05] eq1_freq [unit:Hz] [style:knob] [scale:log]
       [tooltip: Center-frequency of second-order Regalia-Mitra peaking equalizer section 1]",
       315, 40, 2500, 1);

  eq1l = hslider("[06] eq1_level [unit:dB] [style:knob]
       [tooltip: Peak level in dB of second-order Regalia-Mitra peaking equalizer section 1]",
       0, -15, 15, 0.1);

  eq1q = hslider("[07] eq1_q [style:knob]
       [tooltip: Q = centerFrequency/bandwidth of second-order peaking equalizer section 1]",
       3, 0.1, 10, 0.1);

  //eq2_group(x) = hgroup("[08] RM Peaking Equalizer 2", x);

  eq2f = hslider("[09] eq2_freq [unit:Hz] [style:knob] [scale:log]
       [tooltip: Center-frequency of second-order Regalia-Mitra peaking equalizer section 2]",
       1500, 160, 10000, 1);

  eq2l = hslider("[10] eq2_level [unit:dB] [style:knob]
       [tooltip: Peak level in dB of second-order Regalia-Mitra peaking equalizer section 2]",
       0, -15, 15, 0.1);

  eq2q = hslider("[11] Eq2_Q [style:knob]
       [tooltip: Q = centerFrequency/bandwidth of second-order peaking equalizer section 2]",
       3, 0.1, 10, 0.1);

  //dry_wet(x,y) = *(wet) + dry*x, *(wet) + dry*y with {
  //  wet = 0.5*(drywet+1.0);
  //  dry = 1.0-wet;
  //};
  dry_wet(x,y) = *(wet) + dry*x, *(wet) + dry*y with {
    wet = drywet;
    dry = 1.0-wet;
  };

  drywet = hslider("[12] mix [style:knob]
       [tooltip: 0 = dry, 1 = wet]",
       1, 0.0, 1.0, 0.001) : smooth(0.999);

  out_level = *(gain),*(gain);

  gain = hslider("[13] level [unit:dB] [style:knob]
    [tooltip: Output scale factor]", -20, -70, 40, 0.1)
    : db2linear : smooth(0.999);

};

process = my_zita;
