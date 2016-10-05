typedef struct {
    SPFLOAT min, max;
} sp_scale;

int sp_scale_create(sp_scale **p);
int sp_scale_destroy(sp_scale **p);
int sp_scale_init(sp_data *sp, sp_scale *p);
int sp_scale_compute(sp_data *sp, sp_scale *p, SPFLOAT *in, SPFLOAT *out);
