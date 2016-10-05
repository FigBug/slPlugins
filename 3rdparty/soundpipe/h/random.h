typedef struct { 
    SPFLOAT min;
    SPFLOAT max;
} sp_random;

int sp_random_create(sp_random **p);
int sp_random_destroy(sp_random **p);
int sp_random_init(sp_data *sp, sp_random *p);
int sp_random_compute(sp_data *sp, sp_random *p, SPFLOAT *in, SPFLOAT *out);
