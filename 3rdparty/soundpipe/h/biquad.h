typedef struct{
    SPFLOAT b0, b1, b2, a0, a1, a2, reinit, xnm1, xnm2, ynm1, ynm2, cutoff, res;
    SPFLOAT sr;
    SPFLOAT tpidsr;
}sp_biquad;

int sp_biquad_create(sp_biquad **p);
int sp_biquad_destroy(sp_biquad **p);
int sp_biquad_init(sp_data *sp, sp_biquad *p);
int sp_biquad_compute(sp_data *sp, sp_biquad *p, SPFLOAT *in, SPFLOAT *out);
