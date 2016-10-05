typedef struct {
    SPFLOAT atk;
    SPFLOAT dec;
    SPFLOAT sus;
    SPFLOAT rel;
    uint32_t timer;
    uint32_t atk_time;
    SPFLOAT a;
    SPFLOAT b;
    SPFLOAT y;
    SPFLOAT x;
    SPFLOAT prev;
    int mode;
} sp_adsr;

int sp_adsr_create(sp_adsr **p);
int sp_adsr_destroy(sp_adsr **p);
int sp_adsr_init(sp_data *sp, sp_adsr *p);
int sp_adsr_compute(sp_data *sp, sp_adsr *p, SPFLOAT *in, SPFLOAT *out);
