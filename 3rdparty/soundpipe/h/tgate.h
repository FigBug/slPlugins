typedef struct {
    SPFLOAT time;
    uint32_t timer;
} sp_tgate;

int sp_tgate_create(sp_tgate **p);
int sp_tgate_destroy(sp_tgate **p);
int sp_tgate_init(sp_data *sp, sp_tgate *p);
int sp_tgate_compute(sp_data *sp, sp_tgate *p, SPFLOAT *in, SPFLOAT *out);
