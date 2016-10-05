typedef struct {
    void *faust;
    int argpos;
    SPFLOAT *args[11];
    SPFLOAT *in_delay;
    SPFLOAT *lf_x;
    SPFLOAT *rt60_low;
    SPFLOAT *rt60_mid;
    SPFLOAT *hf_damping;
    SPFLOAT *eq1_freq;
    SPFLOAT *eq1_level;
    SPFLOAT *eq2_freq;
    SPFLOAT *eq2_level;
    SPFLOAT *mix;
    SPFLOAT *level;
} sp_zitarev;

int sp_zitarev_create(sp_zitarev **p);
int sp_zitarev_destroy(sp_zitarev **p);
int sp_zitarev_init(sp_data *sp, sp_zitarev *p);
int sp_zitarev_compute(sp_data *sp, sp_zitarev *p, SPFLOAT *in1, SPFLOAT *in2, SPFLOAT *out1, SPFLOAT *out2);

