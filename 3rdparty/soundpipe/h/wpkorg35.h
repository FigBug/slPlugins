typedef struct {
    /* LPF1 */
    SPFLOAT lpf1_a;
    SPFLOAT lpf1_z;
    
    /* LPF2 */
    SPFLOAT lpf2_a;
    SPFLOAT lpf2_b;
    SPFLOAT lpf2_z;
    
    /* HPF */
    SPFLOAT hpf_a;
    SPFLOAT hpf_b;
    SPFLOAT hpf_z;

    SPFLOAT alpha;

    SPFLOAT cutoff;
    SPFLOAT res;
    SPFLOAT saturation;

    SPFLOAT pcutoff;
    SPFLOAT pres;

    uint32_t nonlinear;
} sp_wpkorg35;

int sp_wpkorg35_create(sp_wpkorg35 **p);
int sp_wpkorg35_destroy(sp_wpkorg35 **p);
int sp_wpkorg35_init(sp_data *sp, sp_wpkorg35 *p);
int sp_wpkorg35_compute(sp_data *sp, sp_wpkorg35 *p, SPFLOAT *in, SPFLOAT *out);
