typedef struct {
    int size, pos;
    SPFLOAT *buf;
} sp_sdelay;

int sp_sdelay_create(sp_sdelay **p);
int sp_sdelay_destroy(sp_sdelay **p);
int sp_sdelay_init(sp_data *sp, sp_sdelay *p, int size);
int sp_sdelay_compute(sp_data *sp, sp_sdelay *p, SPFLOAT *in, SPFLOAT *out);
