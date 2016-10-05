typedef struct {
    SPFLOAT fc, v, q, mode;

    SPFLOAT xnm1, xnm2, ynm1, ynm2;
    SPFLOAT prv_fc, prv_v, prv_q;
    SPFLOAT b0, b1, b2, a1, a2;
    SPFLOAT tpidsr;
    int imode;
} sp_pareq;

int sp_pareq_create(sp_pareq **p);
int sp_pareq_destroy(sp_pareq **p);
int sp_pareq_init(sp_data *sp, sp_pareq *p);
int sp_pareq_compute(sp_data *sp, sp_pareq *p, SPFLOAT *in, SPFLOAT *out);
