typedef struct {
    void *ud;
    int argpos;
    SPFLOAT *args[2];
    SPFLOAT *freq;
    SPFLOAT *amp;
} sp_blsaw;

int sp_blsaw_create(sp_blsaw **p);
int sp_blsaw_destroy(sp_blsaw **p);
int sp_blsaw_init(sp_data *sp, sp_blsaw *p);
int sp_blsaw_compute(sp_data *sp, sp_blsaw *p, SPFLOAT *in, SPFLOAT *out);
