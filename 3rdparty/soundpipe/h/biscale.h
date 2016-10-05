typedef struct {
    SPFLOAT min, max;
} sp_biscale;

int sp_biscale_create(sp_biscale **p);
int sp_biscale_destroy(sp_biscale **p);
int sp_biscale_init(sp_data *sp, sp_biscale *p);
int sp_biscale_compute(sp_data *sp, sp_biscale *p, SPFLOAT *in, SPFLOAT *out);
