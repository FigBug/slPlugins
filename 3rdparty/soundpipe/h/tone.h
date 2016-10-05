typedef struct {
    SPFLOAT hp;
    SPFLOAT c1, c2, yt1, prvhp;
    SPFLOAT tpidsr;
} sp_tone;

int sp_tone_create(sp_tone **t);
int sp_tone_destroy(sp_tone **t);
int sp_tone_init(sp_data *sp, sp_tone *t);
int sp_tone_compute(sp_data *sp, sp_tone *t, SPFLOAT *in, SPFLOAT *out);
