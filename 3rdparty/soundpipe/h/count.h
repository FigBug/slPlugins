typedef struct sp_count{
    int32_t count, curcount;
    int mode;
} sp_count;

int sp_count_create(sp_count **p);
int sp_count_destroy(sp_count **p);
int sp_count_init(sp_data *sp, sp_count *p);
int sp_count_compute(sp_data *sp, sp_count *p, SPFLOAT *in, SPFLOAT *out);
