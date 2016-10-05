typedef struct {
    SPFLOAT freq, amp, iphs;
    sp_ftbl *tbl;
    int32_t tablen;
    SPFLOAT tablenUPsr;
    SPFLOAT phs;
    SPFLOAT onedsr;
} sp_posc3;

int sp_posc3_create(sp_posc3 **posc3);
int sp_posc3_destroy(sp_posc3 **posc3);
int sp_posc3_init(sp_data *sp, sp_posc3 *posc3, sp_ftbl *ft);
int sp_posc3_compute(sp_data *sp, sp_posc3 *posc3, SPFLOAT *in, SPFLOAT *out);
