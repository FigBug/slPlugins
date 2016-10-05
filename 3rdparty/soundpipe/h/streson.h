typedef struct{
    SPFLOAT freq, fdbgain;
    SPFLOAT LPdelay, APdelay;
    SPFLOAT *Cdelay;
    sp_auxdata buf;
    int wpointer, rpointer, size;
}sp_streson;

int sp_streson_create(sp_streson **p);
int sp_streson_destroy(sp_streson **p);
int sp_streson_init(sp_data *sp, sp_streson *p);
int sp_streson_compute(sp_data *sp, sp_streson *p, SPFLOAT *in, SPFLOAT *out);
