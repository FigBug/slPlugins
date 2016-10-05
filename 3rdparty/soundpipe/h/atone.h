typedef struct {
    SPFLOAT hp;
    SPFLOAT c1, c2, yt1, prvhp;
    SPFLOAT tpidsr;
} sp_atone;

int sp_atone_create(sp_atone **p);
int sp_atone_destroy(sp_atone **p);
int sp_atone_init(sp_data *sp, sp_atone *p);
int sp_atone_compute(sp_data *sp, sp_atone *p, SPFLOAT *in, SPFLOAT *out);
