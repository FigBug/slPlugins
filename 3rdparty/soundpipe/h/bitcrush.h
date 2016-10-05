typedef struct {
    SPFLOAT bitdepth;
    SPFLOAT srate;
    sp_fold *fold;
} sp_bitcrush;

int sp_bitcrush_create(sp_bitcrush **p);
int sp_bitcrush_destroy(sp_bitcrush **p);
int sp_bitcrush_init(sp_data *sp, sp_bitcrush *p);
int sp_bitcrush_compute(sp_data *sp, sp_bitcrush *p, SPFLOAT *in, SPFLOAT *out);
