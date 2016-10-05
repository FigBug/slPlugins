typedef struct {
    SPFLOAT freq, bw;
    int scale;
    SPFLOAT c1, c2, c3, yt1, yt2, cosf, prvfreq, prvbw;
    SPFLOAT tpidsr;
} sp_reson;

int sp_reson_create(sp_reson **p);
int sp_reson_destroy(sp_reson **p);
int sp_reson_init(sp_data *sp, sp_reson *p);
int sp_reson_compute(sp_data *sp, sp_reson *p, SPFLOAT *in, SPFLOAT *out);
