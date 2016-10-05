typedef struct{
    SPFLOAT freq, q, xnm1, ynm1, ynm2, a0, a1, a2, d, lfq, lq;
    SPFLOAT sr;
}sp_mode;

int sp_mode_create(sp_mode **p);
int sp_mode_destroy(sp_mode **p);
int sp_mode_init(sp_data *sp, sp_mode *p);
int sp_mode_compute(sp_data *sp, sp_mode *p, SPFLOAT *in, SPFLOAT *out);
