typedef struct sp_dtrig{
    sp_ftbl *ft;
    uint32_t counter;
    uint32_t pos;
    int running;
    int loop;
    SPFLOAT delay;
    SPFLOAT scale;
} sp_dtrig;

int sp_dtrig_create(sp_dtrig **p);
int sp_dtrig_destroy(sp_dtrig **p);
int sp_dtrig_init(sp_data *sp, sp_dtrig *p, sp_ftbl *ft);
int sp_dtrig_compute(sp_data *sp, sp_dtrig *p, SPFLOAT *in, SPFLOAT *out);
