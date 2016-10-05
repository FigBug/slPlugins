typedef struct {
    SPFLOAT value;
    SPFLOAT target;
    SPFLOAT rate;
    int state;
    SPFLOAT attackRate;
    SPFLOAT decayRate;
    SPFLOAT sustainLevel;
    SPFLOAT releaseRate;
    SPFLOAT atk;
    SPFLOAT rel;
    SPFLOAT sus;
    SPFLOAT dec;
    int mode;
} sp_tadsr;

int sp_tadsr_create(sp_tadsr **p);
int sp_tadsr_destroy(sp_tadsr **p);
int sp_tadsr_init(sp_data *sp, sp_tadsr *p);
int sp_tadsr_compute(sp_data *sp, sp_tadsr *p, SPFLOAT *trig, SPFLOAT *out);
