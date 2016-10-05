typedef struct {
    SNDFILE *file;
    SF_INFO info;
    SPFLOAT buffer[1024];
    int bufpos;
    int loaded;
    int count;
} sp_diskin;

int sp_diskin_create(sp_diskin **p);
int sp_diskin_destroy(sp_diskin **p);
int sp_diskin_init(sp_data *sp, sp_diskin *p, const char *filename);
int sp_diskin_compute(sp_data *sp, sp_diskin *p, SPFLOAT *in, SPFLOAT *out);
