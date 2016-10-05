typedef struct {
    SPFLOAT revtime, looptime;
    SPFLOAT coef, prvt;
    sp_auxdata aux;
    uint32_t bufpos;
    uint32_t bufsize;
} sp_allpass;

int sp_allpass_create(sp_allpass **p);
int sp_allpass_destroy(sp_allpass **p);
int sp_allpass_init(sp_data *sp, sp_allpass *p, SPFLOAT looptime);
int sp_allpass_compute(sp_data *sp, sp_allpass *p, SPFLOAT *in, SPFLOAT *out);
