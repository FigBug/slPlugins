typedef struct {
    void *faust;
    int argpos;
    SPFLOAT *args[3];
    SPFLOAT *atk;
    SPFLOAT *rel;
    SPFLOAT *bwratio;
} sp_vocoder;

int sp_vocoder_create(sp_vocoder **p);
int sp_vocoder_destroy(sp_vocoder **p);
int sp_vocoder_init(sp_data *sp, sp_vocoder *p);
int sp_vocoder_compute(sp_data *sp, sp_vocoder *p, SPFLOAT *source, SPFLOAT *excite, SPFLOAT *out);

