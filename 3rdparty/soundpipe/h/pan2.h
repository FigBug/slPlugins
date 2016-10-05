typedef struct {
    SPFLOAT pan;
    uint32_t type;
} sp_pan2;

int sp_pan2_create(sp_pan2 **p);
int sp_pan2_destroy(sp_pan2 **p);
int sp_pan2_init(sp_data *sp, sp_pan2 *p);
int sp_pan2_compute(sp_data *sp, sp_pan2 *p, SPFLOAT *in, SPFLOAT *out1, SPFLOAT *out2);
