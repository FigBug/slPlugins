typedef struct {
        SPFLOAT amp, freq, nharm, lharm, mul, iphs;
        int16_t ampcod, cpscod, prvn;
        SPFLOAT prvr, twor, rsqp1, rtn, rtnp1, rsumr;
        int32_t lphs;
        int reported;
        SPFLOAT last;
        sp_ftbl *ft;
} sp_gbuzz;

int sp_gbuzz_create(sp_gbuzz **p);
int sp_gbuzz_destroy(sp_gbuzz **p);
int sp_gbuzz_init(sp_data *sp, sp_gbuzz *p, sp_ftbl *ft, SPFLOAT iphs);
int sp_gbuzz_compute(sp_data *sp, sp_gbuzz *p, SPFLOAT *in, SPFLOAT *out);
