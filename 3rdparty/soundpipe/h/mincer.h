typedef struct {
    SPFLOAT time, amp, pitch, lock, iN,
        idecim, onset, offset, dbthresh;
    int cnt, hsize, curframe, N, decim,tscale;
    SPFLOAT pos;
    SPFLOAT accum;
    sp_auxdata outframe, win, bwin, fwin,
    nwin, prev, framecount, indata[2];
    SPFLOAT *tab;
    int curbuf;
    SPFLOAT resamp;
    sp_ftbl *ft;
    sp_fft fft;
} sp_mincer;

int sp_mincer_create(sp_mincer **p);
int sp_mincer_destroy(sp_mincer **p);
int sp_mincer_init(sp_data *sp, sp_mincer *p, sp_ftbl *ft);
int sp_mincer_compute(sp_data *sp, sp_mincer *p, SPFLOAT *in, SPFLOAT *out);
