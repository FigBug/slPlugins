typedef struct sp_bal{
    SPFLOAT asig, csig, ihp;
    SPFLOAT c1, c2, prvq, prvr, prva;
} sp_bal;

int sp_bal_create(sp_bal **p);
int sp_bal_destroy(sp_bal **p);
int sp_bal_init(sp_data *sp, sp_bal *p);
int sp_bal_compute(sp_data *sp, sp_bal *p, SPFLOAT *sig, SPFLOAT *comp, SPFLOAT *out);
