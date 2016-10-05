typedef struct sp_fof_overlap {
        struct sp_fof_overlap *nxtact, *nxtfree;
        int32_t timrem, dectim, formphs, forminc, risphs, risinc, decphs, decinc;
        SPFLOAT curamp, expamp;
        SPFLOAT glissbas;
        int32_t sampct;
} sp_fof_overlap;

typedef struct {
    SPFLOAT amp, fund, form, oct, band, ris, dur, dec;
    SPFLOAT iolaps, iphs;
    int32_t durtogo, fundphs, fofcount, prvsmps;
    SPFLOAT prvband, expamp, preamp;
    int16_t foftype;        
    int16_t xincod, ampcod, fundcod, formcod, fmtmod;
    sp_auxdata auxch;
    sp_ftbl *ftp1, *ftp2;
    sp_fof_overlap basovrlap;
} sp_fof;

int sp_fof_create(sp_fof **p);
int sp_fof_destroy(sp_fof **p);
int sp_fof_init(sp_data *sp, sp_fof *p, sp_ftbl *sine, sp_ftbl *win, int iolaps, SPFLOAT iphs);
int sp_fof_compute(sp_data *sp, sp_fof *p, SPFLOAT *in, SPFLOAT *out);
