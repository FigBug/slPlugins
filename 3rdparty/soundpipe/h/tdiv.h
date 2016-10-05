typedef struct {
    uint32_t num, counter, offset;
} sp_tdiv;

int sp_tdiv_create(sp_tdiv **p);
int sp_tdiv_destroy(sp_tdiv **p);
int sp_tdiv_init(sp_data *sp, sp_tdiv *p);
int sp_tdiv_compute(sp_data *sp, sp_tdiv *p, SPFLOAT *in, SPFLOAT *out);
