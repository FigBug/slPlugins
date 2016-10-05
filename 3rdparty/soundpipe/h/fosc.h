typedef struct sp_fosc{
    SPFLOAT amp, freq, car, mod, indx, iphs;
    int32_t mphs, cphs;
    sp_ftbl *ft;
} sp_fosc;

int sp_fosc_create(sp_fosc **p);
int sp_fosc_destroy(sp_fosc **p);
int sp_fosc_init(sp_data *sp, sp_fosc *p, sp_ftbl *ft);
int sp_fosc_compute(sp_data *sp, sp_fosc *p, SPFLOAT *in, SPFLOAT *out);
