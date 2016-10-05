typedef struct sp_tenvx{
    SPFLOAT atk, rel, hold;
    SPFLOAT patk, prel;
    uint32_t count;
    SPFLOAT a_a, b_a;
    SPFLOAT a_r, b_r;
    SPFLOAT y;
} sp_tenvx;

int sp_tenvx_create(sp_tenvx **p);
int sp_tenvx_destroy(sp_tenvx **p);
int sp_tenvx_init(sp_data *sp, sp_tenvx *p);
int sp_tenvx_compute(sp_data *sp, sp_tenvx *p, SPFLOAT *in, SPFLOAT *out);
