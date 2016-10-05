typedef struct {
    SPFLOAT mode;
} sp_switch;

int sp_switch_create(sp_switch **p);
int sp_switch_destroy(sp_switch **p);
int sp_switch_init(sp_data *sp, sp_switch *p);
int sp_switch_compute(sp_data *sp, sp_switch *p, SPFLOAT *trig,
    SPFLOAT *in1, SPFLOAT *in2, SPFLOAT *out);
