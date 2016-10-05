typedef struct {
    void *ud;
    int argpos;
    SPFLOAT *args[2];
    SPFLOAT *freq;
    SPFLOAT *amp;
} sp_bltriangle;

int sp_bltriangle_create(sp_bltriangle **p);
int sp_bltriangle_destroy(sp_bltriangle **p);
int sp_bltriangle_init(sp_data *sp, sp_bltriangle *p);
int sp_bltriangle_compute(sp_data *sp, sp_bltriangle *p, SPFLOAT *in, SPFLOAT *out);

