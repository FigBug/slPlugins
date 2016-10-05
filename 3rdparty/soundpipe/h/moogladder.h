typedef struct {
    SPFLOAT freq;
    SPFLOAT res;
    SPFLOAT istor;

    SPFLOAT delay[6];
    SPFLOAT tanhstg[3];
    SPFLOAT oldfreq;
    SPFLOAT oldres;
    SPFLOAT oldacr;
    SPFLOAT oldtune;
} sp_moogladder;

int sp_moogladder_create(sp_moogladder **t);
int sp_moogladder_destroy(sp_moogladder **t);
int sp_moogladder_init(sp_data *sp, sp_moogladder *p);
int sp_moogladder_compute(sp_data *sp, sp_moogladder *p, SPFLOAT *in, SPFLOAT *out);
