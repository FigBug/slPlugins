typedef struct {
    int state;
    SPFLOAT atk, rel;
    uint32_t totaltime;
    uint32_t timer;
    SPFLOAT slope;
    SPFLOAT last;
} sp_tenv2;

int sp_tenv2_create(sp_tenv2 **p);
int sp_tenv2_destroy(sp_tenv2 **p);
int sp_tenv2_init(sp_data *sp, sp_tenv2 *p);
int sp_tenv2_compute(sp_data *sp, sp_tenv2 *p, SPFLOAT *in, SPFLOAT *out);
