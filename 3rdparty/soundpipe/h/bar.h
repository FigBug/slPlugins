typedef struct {
    SPFLOAT bcL, bcR, iK, ib, scan, T30;
    SPFLOAT pos, vel, wid;

    SPFLOAT *w, *w1, *w2;
    int step, first;
    SPFLOAT s0, s1, s2, t0, t1;
    int i_bcL, i_bcR, N;
    sp_auxdata w_aux;
} sp_bar;

int sp_bar_create(sp_bar **p);
int sp_bar_destroy(sp_bar **p);
int sp_bar_init(sp_data *sp, sp_bar *p, SPFLOAT iK, SPFLOAT ib);
int sp_bar_compute(sp_data *sp, sp_bar *p, SPFLOAT *in, SPFLOAT *out);
