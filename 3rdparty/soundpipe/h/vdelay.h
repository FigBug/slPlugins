typedef struct sp_vdelay{
    SPFLOAT del, maxdel;
    SPFLOAT sr;
    sp_auxdata buf;
    int32_t left;
} sp_vdelay;

int sp_vdelay_create(sp_vdelay **p);
int sp_vdelay_destroy(sp_vdelay **p);
int sp_vdelay_init(sp_data *sp, sp_vdelay *p, SPFLOAT maxdel);
int sp_vdelay_compute(sp_data *sp, sp_vdelay *p, SPFLOAT *in, SPFLOAT *out);
