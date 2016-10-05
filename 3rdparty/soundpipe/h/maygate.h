typedef struct sp_maygate{
    SPFLOAT prob;
    SPFLOAT gate;
    int mode;
} sp_maygate;

int sp_maygate_create(sp_maygate **p);
int sp_maygate_destroy(sp_maygate **p);
int sp_maygate_init(sp_data *sp, sp_maygate *p);
int sp_maygate_compute(sp_data *sp, sp_maygate *p, SPFLOAT *in, SPFLOAT *out);
