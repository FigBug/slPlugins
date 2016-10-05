typedef struct{
    SPFLOAT lim, k1;
}sp_clip;

int sp_clip_create(sp_clip **p);
int sp_clip_destroy(sp_clip **p);
int sp_clip_init(sp_data *sp, sp_clip *p);
int sp_clip_compute(sp_data *sp, sp_clip *p, SPFLOAT *in, SPFLOAT *out);
