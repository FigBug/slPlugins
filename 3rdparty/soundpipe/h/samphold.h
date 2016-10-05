typedef struct {
    SPFLOAT val;
} sp_samphold;

int sp_samphold_create(sp_samphold **p);
int sp_samphold_destroy(sp_samphold **p);
int sp_samphold_init(sp_data *sp, sp_samphold *p);
int sp_samphold_compute(sp_data *sp, sp_samphold *p, SPFLOAT *trig, SPFLOAT *in, SPFLOAT *out);
