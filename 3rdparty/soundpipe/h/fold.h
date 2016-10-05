typedef struct {
    SPFLOAT incr;
    SPFLOAT index;
    int32_t sample_index;
    SPFLOAT value;
} sp_fold;

int sp_fold_create(sp_fold **p);
int sp_fold_destroy(sp_fold **p);
int sp_fold_init(sp_data *sp, sp_fold *p);
int sp_fold_compute(sp_data *sp, sp_fold *p, SPFLOAT *in, SPFLOAT *out);
