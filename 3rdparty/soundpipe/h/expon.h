typedef struct {
    SPFLOAT a, dur, b;
    SPFLOAT val, incr; 
    uint32_t sdur, stime;
    int init;
} sp_expon;

int sp_expon_create(sp_expon **p);
int sp_expon_destroy(sp_expon **p);
int sp_expon_init(sp_data *sp, sp_expon *p);
int sp_expon_compute(sp_data *sp, sp_expon *p, SPFLOAT *in, SPFLOAT *out);
