typedef struct {
    SPFLOAT amp, freq, ifreq;
    SPFLOAT sicps;
    int32_t phs256, npts, maxpts;
    sp_auxdata auxch;
    char init;
} sp_pluck;

int sp_pluck_create(sp_pluck **p);
int sp_pluck_destroy(sp_pluck **p);
int sp_pluck_init(sp_data *sp, sp_pluck *p, SPFLOAT ifreq);
int sp_pluck_compute(sp_data *sp, sp_pluck *p, SPFLOAT *trig, SPFLOAT *out);
