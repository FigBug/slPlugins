typedef struct {
    SPFLOAT freq;
    SPFLOAT min, max;
    SPFLOAT val;
    uint32_t counter, dur;
} sp_randh;

int sp_randh_create(sp_randh **p);
int sp_randh_destroy(sp_randh **p);
int sp_randh_init(sp_data *sp, sp_randh *p);
int sp_randh_compute(sp_data *sp, sp_randh *p, SPFLOAT *in, SPFLOAT *out);
