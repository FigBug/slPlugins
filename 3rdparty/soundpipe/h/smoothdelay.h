typedef struct {
    SPFLOAT del, maxdel, pdel;
    SPFLOAT sr;
    SPFLOAT feedback;

    int counter;
    int maxcount;

    uint32_t maxbuf;

    sp_auxdata buf1;
    uint32_t bufpos1;
    uint32_t deltime1;

    sp_auxdata buf2;
    uint32_t bufpos2;
    uint32_t deltime2;
    int curbuf;
} sp_smoothdelay;

int sp_smoothdelay_create(sp_smoothdelay **p);
int sp_smoothdelay_destroy(sp_smoothdelay **p);
int sp_smoothdelay_init(sp_data *sp, sp_smoothdelay *p, 
        SPFLOAT maxdel, uint32_t interp);
int sp_smoothdelay_compute(sp_data *sp, sp_smoothdelay *p, SPFLOAT *in, SPFLOAT *out);
