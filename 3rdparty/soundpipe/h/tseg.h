typedef struct {
    SPFLOAT beg,dur,end;
    uint32_t steps;
    uint32_t count;
    SPFLOAT val;
    SPFLOAT type;
    SPFLOAT slope;
    SPFLOAT tdivnsteps;
} sp_tseg;

int sp_tseg_create(sp_tseg **p);
int sp_tseg_destroy(sp_tseg **p);
int sp_tseg_init(sp_data *sp, sp_tseg *p, SPFLOAT ibeg);
int sp_tseg_compute(sp_data *sp, sp_tseg *p, SPFLOAT *in, SPFLOAT *out);
