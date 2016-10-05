typedef struct {
    FILE *fp;
    SPFLOAT val;
} sp_tin;

int sp_tin_create(sp_tin **p);
int sp_tin_destroy(sp_tin **p);
int sp_tin_init(sp_data *sp, sp_tin *p);
int sp_tin_compute(sp_data *sp, sp_tin *p, SPFLOAT *in, SPFLOAT *out);
