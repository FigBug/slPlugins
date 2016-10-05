typedef struct sp_dist{
    SPFLOAT pregain, postgain, shape1, shape2, mode;
} sp_dist;

int sp_dist_create(sp_dist **p);
int sp_dist_destroy(sp_dist **p);
int sp_dist_init(sp_data *sp, sp_dist *p);
int sp_dist_compute(sp_data *sp, sp_dist *p, SPFLOAT *in, SPFLOAT *out);
