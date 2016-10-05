typedef struct {
    void *ud;
} sp_jcrev;

int sp_jcrev_create(sp_jcrev **p);
int sp_jcrev_destroy(sp_jcrev **p);
int sp_jcrev_init(sp_data *sp, sp_jcrev *p);
int sp_jcrev_compute(sp_data *sp, sp_jcrev *p, SPFLOAT *in, SPFLOAT *out);
