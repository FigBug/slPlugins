typedef struct  {
    SPFLOAT delay;
    uint32_t bufpos;
    uint32_t bufsize;
    sp_auxdata buf;
} sp_reverse;

int sp_reverse_create(sp_reverse **p);
int sp_reverse_destroy(sp_reverse **p); 
int sp_reverse_init(sp_data *sp, sp_reverse *p, SPFLOAT delay);
int sp_reverse_compute(sp_data *sp, sp_reverse *p, SPFLOAT *in, SPFLOAT *out);
