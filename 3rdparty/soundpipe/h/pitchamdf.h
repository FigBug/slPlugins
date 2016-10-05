typedef struct {
    SPFLOAT imincps, imaxcps, icps;
    SPFLOAT imedi, idowns, iexcps, irmsmedi;
    SPFLOAT srate;
    SPFLOAT lastval;
    int32_t downsamp;
    int32_t upsamp;
    int32_t minperi;
    int32_t maxperi;
    int32_t index;
    int32_t readp;
    int32_t size;
    int32_t peri;
    int32_t medisize;
    int32_t mediptr;
    int32_t rmsmedisize;
    int32_t rmsmediptr;
    int inerr;
    sp_auxdata median;
    sp_auxdata rmsmedian;
    sp_auxdata buffer;
} sp_pitchamdf;

int sp_pitchamdf_create(sp_pitchamdf **p);
int sp_pitchamdf_destroy(sp_pitchamdf **p);
int sp_pitchamdf_init(sp_data *sp, sp_pitchamdf *p, SPFLOAT imincps, SPFLOAT imaxcps);
int sp_pitchamdf_compute(sp_data *sp, sp_pitchamdf *p, SPFLOAT *in, SPFLOAT *cps, SPFLOAT *rms);
