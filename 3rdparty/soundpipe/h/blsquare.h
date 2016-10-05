typedef struct {
    void *ud;
    int argpos;
    SPFLOAT *args[3];
    SPFLOAT *freq;
    SPFLOAT *amp;
    SPFLOAT *width;
} sp_blsquare;

int sp_blsquare_create(sp_blsquare **p);
int sp_blsquare_destroy(sp_blsquare **p);
int sp_blsquare_init(sp_data *sp, sp_blsquare *p);
int sp_blsquare_compute(sp_data *sp, sp_blsquare *p, SPFLOAT *in, SPFLOAT *out);
