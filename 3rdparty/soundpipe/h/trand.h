typedef struct {
    SPFLOAT min, max, val;
} sp_trand;

int sp_trand_create(sp_trand **p);
int sp_trand_destroy(sp_trand **p);
int sp_trand_init(sp_data *sp, sp_trand *p);
int sp_trand_compute(sp_data *sp, sp_trand *p, SPFLOAT *in, SPFLOAT *out);
