typedef struct {
    void *faust;
    int argpos;
    SPFLOAT *args[4];
    SPFLOAT *ratio;
    SPFLOAT *thresh;
    SPFLOAT *atk;
    SPFLOAT *rel;
} sp_compressor;

int sp_compressor_create(sp_compressor **p);
int sp_compressor_destroy(sp_compressor **p);
int sp_compressor_init(sp_data *sp, sp_compressor *p);
int sp_compressor_compute(sp_data *sp, sp_compressor *p, SPFLOAT *in, SPFLOAT *out);
