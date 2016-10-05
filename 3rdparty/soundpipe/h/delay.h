typedef struct {
    SPFLOAT time;
    SPFLOAT feedback;
    SPFLOAT last;
    sp_auxdata buf;
    uint32_t bufsize;
    uint32_t bufpos;
} sp_delay;

int sp_delay_create(sp_delay **p);
int sp_delay_destroy(sp_delay **p);
int sp_delay_init(sp_data *sp, sp_delay *p, SPFLOAT time);
int sp_delay_compute(sp_data *sp, sp_delay *p, SPFLOAT *in, SPFLOAT *out);
