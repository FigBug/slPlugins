typedef struct sp_padsynth {
    SPFLOAT cps;
    SPFLOAT bw;
    sp_ftbl *amps;
} sp_padsynth;

int sp_gen_padsynth(sp_data *sp, sp_ftbl *ps, sp_ftbl *amps, SPFLOAT f, SPFLOAT bw);

SPFLOAT sp_padsynth_profile(SPFLOAT fi, SPFLOAT bwi);

int sp_padsynth_ifft(int N, SPFLOAT *freq_amp, 
        SPFLOAT *freq_phase, SPFLOAT *smp); 

int sp_padsynth_normalize(int N, SPFLOAT *smp);
