typedef struct {
    sp_ftbl *vals;
    sp_ftbl *buf;
    uint32_t id;
    uint32_t pos;
    uint32_t nextpos;
} sp_slice;

int sp_slice_create(sp_slice **p);
int sp_slice_destroy(sp_slice **p);
int sp_slice_init(sp_data *sp, sp_slice *p, sp_ftbl *vals, sp_ftbl *buf);
int sp_slice_compute(sp_data *sp, sp_slice *p, SPFLOAT *in, SPFLOAT *out);
