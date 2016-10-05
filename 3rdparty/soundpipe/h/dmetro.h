typedef struct {
    SPFLOAT time;
    uint32_t counter;
} sp_dmetro;

int sp_dmetro_create(sp_dmetro **p);
int sp_dmetro_destroy(sp_dmetro **p);
int sp_dmetro_init(sp_data *sp, sp_dmetro *p);
int sp_dmetro_compute(sp_data *sp, sp_dmetro *p, SPFLOAT *in, SPFLOAT *out);
