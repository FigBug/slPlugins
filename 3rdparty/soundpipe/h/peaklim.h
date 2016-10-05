typedef struct {
    SPFLOAT atk, rel, thresh;
    SPFLOAT patk, prel;
	SPFLOAT b0_r, a1_r, b0_a, a1_a, level;
} sp_peaklim;

int sp_peaklim_create(sp_peaklim **p);
int sp_peaklim_destroy(sp_peaklim **p);
int sp_peaklim_init(sp_data *sp, sp_peaklim *p);
int sp_peaklim_compute(sp_data *sp, sp_peaklim *p, SPFLOAT *in, SPFLOAT *out);
