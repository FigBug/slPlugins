typedef struct {
    SPFLOAT pan;
    uint32_t type;
} sp_panst;

int sp_panst_create(sp_panst **p);
int sp_panst_destroy(sp_panst **p);
int sp_panst_init(sp_data *sp, sp_panst *p);
int sp_panst_compute(sp_data *sp, sp_panst *p, SPFLOAT *in1, SPFLOAT *in2, SPFLOAT *out1, SPFLOAT *out2);
