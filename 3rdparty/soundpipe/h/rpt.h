typedef struct sp_rpt{
    uint32_t playpos;
    uint32_t bufpos;
    int running;
    int count, reps;
    SPFLOAT sr;
    uint32_t size;
    SPFLOAT bpm;
    int div, rep;
    sp_auxdata aux;
    int rc;
} sp_rpt;

int sp_rpt_create(sp_rpt **p);
int sp_rpt_destroy(sp_rpt **p);
int sp_rpt_init(sp_data *sp, sp_rpt *p, SPFLOAT maxdur);
int sp_rpt_compute(sp_data *sp, sp_rpt *p, SPFLOAT *trig, 
        SPFLOAT *in, SPFLOAT *out);
