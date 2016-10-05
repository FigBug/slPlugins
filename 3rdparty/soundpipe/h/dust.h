typedef struct sp_dust{
    SPFLOAT amp, density; 
    SPFLOAT density0, thresh, scale;
    SPFLOAT onedsr;
    int bipolar; /* 1 = bipolar 0 = unipolar */
    uint32_t rand;
} sp_dust;

int sp_dust_create(sp_dust **p);
int sp_dust_destroy(sp_dust **p);
int sp_dust_init(sp_data *sp, sp_dust *p);
int sp_dust_compute(sp_data *sp, sp_dust *p, SPFLOAT *in, SPFLOAT *out);


