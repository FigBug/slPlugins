typedef struct {
    int mode;
    uint32_t pos;
    SPFLOAT time;
} sp_timer;

int sp_timer_create(sp_timer **p);
int sp_timer_destroy(sp_timer **p);
int sp_timer_init(sp_data *sp, sp_timer *p);
int sp_timer_compute(sp_data *sp, sp_timer *p, SPFLOAT *in, SPFLOAT *out);
