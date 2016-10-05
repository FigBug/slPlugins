typedef struct sp_jitter{
    SPFLOAT amp, cpsMin, cpsMax;
    SPFLOAT cps;
    int32_t phs;
    int initflag;
    SPFLOAT num1, num2, dfdmax;
} sp_jitter;

int sp_jitter_create(sp_jitter **p);
int sp_jitter_destroy(sp_jitter **p);
int sp_jitter_init(sp_data *sp, sp_jitter *p);
int sp_jitter_compute(sp_data *sp, sp_jitter *p, SPFLOAT *in, SPFLOAT *out);
