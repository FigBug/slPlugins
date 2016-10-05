typedef struct sp_fog_overlap {
    struct sp_fog_overlap *nxtact;
    struct sp_fog_overlap *nxtfree;
    int32_t timrem, dectim, formphs, forminc;
    uint32_t risphs;
    int32_t risinc, decphs, decinc;
    SPFLOAT curamp, expamp;
    SPFLOAT pos, inc;
} sp_fog_overlap;

typedef struct {
    SPFLOAT amp, dens, trans, spd, oct, band, ris, dur, dec;
    SPFLOAT iolaps, iphs, itmode;
    sp_fog_overlap basovrlap;
    int32_t durtogo, fundphs, fofcount, prvsmps, spdphs;
    SPFLOAT prvband, expamp, preamp, fogcvt; 
    int16_t formcod, fmtmod, speedcod;
    sp_auxdata auxch;
    sp_ftbl *ftp1, *ftp2;
} sp_fog;

int sp_fog_create(sp_fog **p);
int sp_fog_destroy(sp_fog **p);
int sp_fog_init(sp_data *sp, sp_fog *p, sp_ftbl *wav, sp_ftbl *win, int iolaps, SPFLOAT iphs);
int sp_fog_compute(sp_data *sp, sp_fog *p, SPFLOAT *in, SPFLOAT *out);
