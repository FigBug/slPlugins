typedef struct {
    SPFLOAT pos;
} sp_crossfade;

int sp_crossfade_create(sp_crossfade **p);
int sp_crossfade_destroy(sp_crossfade **p);
int sp_crossfade_init(sp_data *sp, sp_crossfade *p);
int sp_crossfade_compute(sp_data *sp, sp_crossfade *p, SPFLOAT *in1, SPFLOAT *in2, SPFLOAT *out);
