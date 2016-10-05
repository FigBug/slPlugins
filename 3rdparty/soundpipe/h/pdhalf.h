typedef struct {
    SPFLOAT amount, ibipolar, ifullscale;
} sp_pdhalf;

int sp_pdhalf_create(sp_pdhalf **p);
int sp_pdhalf_destroy(sp_pdhalf **p);
int sp_pdhalf_init(sp_data *sp, sp_pdhalf *p);
int sp_pdhalf_compute(sp_data *sp, sp_pdhalf *p, SPFLOAT *in, SPFLOAT *out);
