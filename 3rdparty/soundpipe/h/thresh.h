typedef struct {
    int mode, init;
    SPFLOAT prev, thresh;
} sp_thresh;

int sp_thresh_create(sp_thresh **p);
int sp_thresh_destroy(sp_thresh **p);
int sp_thresh_init(sp_data *sp, sp_thresh *p);
int sp_thresh_compute(sp_data *sp, sp_thresh *p, SPFLOAT *in, SPFLOAT *out);
