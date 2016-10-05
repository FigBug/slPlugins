typedef struct {
    SPFLOAT min, max, cps, mode, fstval;
    int16_t cpscod;
    int32_t phs;
    SPFLOAT num1, num2, dfdmax;
    int holdrand;
    SPFLOAT sicvt;
} sp_randi;

int sp_randi_create(sp_randi **p);
int sp_randi_destroy(sp_randi **p);
int sp_randi_init(sp_data *sp, sp_randi *p);
int sp_randi_compute(sp_data *sp, sp_randi *p, SPFLOAT *in, SPFLOAT *out);
