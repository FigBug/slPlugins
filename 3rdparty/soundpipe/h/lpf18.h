typedef struct sp_lpf18{
    SPFLOAT cutoff, res, dist;
    SPFLOAT ay1, ay2, aout, lastin, onedsr;
} sp_lpf18;

int sp_lpf18_create(sp_lpf18 **p);
int sp_lpf18_destroy(sp_lpf18 **p);
int sp_lpf18_init(sp_data *sp, sp_lpf18 *p);
int sp_lpf18_compute(sp_data *sp, sp_lpf18 *p, SPFLOAT *in, SPFLOAT *out);
