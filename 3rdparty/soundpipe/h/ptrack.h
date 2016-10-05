typedef struct {
    SPFLOAT freq, amp;
    SPFLOAT asig,size,peak;
    sp_auxdata signal, prev, sin, spec1, spec2, peakarray;
    int numpks;
    int cnt;
    int histcnt;
    int hopsize;
    SPFLOAT sr;
    SPFLOAT cps;
    SPFLOAT dbs[20];
    SPFLOAT amplo;
    SPFLOAT amphi;
    SPFLOAT npartial;
    SPFLOAT dbfs;
    SPFLOAT prevf;
    sp_fft fft;
} sp_ptrack;

int sp_ptrack_create(sp_ptrack **p);
int sp_ptrack_destroy(sp_ptrack **p);
int sp_ptrack_init(sp_data *sp, sp_ptrack *p, int ihopsize, int ipeaks);
int sp_ptrack_compute(sp_data *sp, sp_ptrack *p, SPFLOAT *in, SPFLOAT *freq, SPFLOAT *amp);
