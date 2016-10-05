typedef struct {
    SPFLOAT xnm1[12], ynm1[12], coef[12];
} sp_hilbert;

int sp_hilbert_create(sp_hilbert **p);
int sp_hilbert_destroy(sp_hilbert **p);
int sp_hilbert_init(sp_data *sp, sp_hilbert *p);
int sp_hilbert_compute(sp_data *sp, sp_hilbert *p, SPFLOAT *in, SPFLOAT *out1, SPFLOAT *out2);
