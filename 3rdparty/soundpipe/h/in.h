typedef struct {
    FILE *fp;
} sp_in;

int sp_in_create(sp_in **p);
int sp_in_destroy(sp_in **p);
int sp_in_init(sp_data *sp, sp_in *p);
int sp_in_compute(sp_data *sp, sp_in *p, SPFLOAT *in, SPFLOAT *out);
