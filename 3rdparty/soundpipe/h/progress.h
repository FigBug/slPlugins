typedef struct {
    int nbars, skip;
    int counter;
    uint32_t len;
} sp_progress;

int sp_progress_create(sp_progress **p);
int sp_progress_destroy(sp_progress **p);
int sp_progress_init(sp_data *sp, sp_progress *p);
int sp_progress_compute(sp_data *sp, sp_progress *p, SPFLOAT *in, SPFLOAT *out);
