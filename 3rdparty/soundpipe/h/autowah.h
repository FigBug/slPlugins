typedef struct {
    void *faust;
    int argpos;
    SPFLOAT *args[3];
    SPFLOAT *level;
    SPFLOAT *wah;
    SPFLOAT *mix;
} sp_autowah;

int sp_autowah_create(sp_autowah **p);
int sp_autowah_destroy(sp_autowah **p);
int sp_autowah_init(sp_data *sp, sp_autowah *p);
int sp_autowah_compute(sp_data *sp, sp_autowah *p, SPFLOAT *in, SPFLOAT *out);
