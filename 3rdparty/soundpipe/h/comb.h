typedef struct sp_comb{
    SPFLOAT revtime, looptime;
    SPFLOAT coef, prvt;
    sp_auxdata aux;
    uint32_t bufpos;
    uint32_t bufsize;
} sp_comb;

int sp_comb_create(sp_comb **p);
int sp_comb_destroy(sp_comb **p);
int sp_comb_init(sp_data *sp, sp_comb *p, SPFLOAT looptime);
int sp_comb_compute(sp_data *sp, sp_comb *p, SPFLOAT *in, SPFLOAT *out);
