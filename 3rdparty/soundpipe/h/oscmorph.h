typedef struct {
    SPFLOAT freq, amp, iphs;
    int32_t lphs;
    sp_ftbl **tbl;
    int inc;
    SPFLOAT wtpos;
    int nft;
} sp_oscmorph;

int sp_oscmorph_create(sp_oscmorph **p);
int sp_oscmorph_destroy(sp_oscmorph **p);
int sp_oscmorph_init(sp_data *sp, sp_oscmorph *osc, sp_ftbl **ft, int nft, SPFLOAT iphs);
int sp_oscmorph_compute(sp_data *sp, sp_oscmorph *p, SPFLOAT *in, SPFLOAT *out);
