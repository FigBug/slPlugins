typedef struct {
    SPFLOAT amp;
    unsigned int newrand;
    unsigned int prevrand;
    unsigned int k;
    unsigned int seed;
    unsigned int total;
    uint32_t counter;
    unsigned int dice[7];
} sp_pinknoise;

int sp_pinknoise_create(sp_pinknoise **p);
int sp_pinknoise_destroy(sp_pinknoise **p);
int sp_pinknoise_init(sp_data *sp, sp_pinknoise *p);
int sp_pinknoise_compute(sp_data *sp, sp_pinknoise *p, SPFLOAT *in, SPFLOAT *out);

