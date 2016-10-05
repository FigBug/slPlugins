typedef struct {
    SPFLOAT rep, len;
    sp_auxdata auxch;
    int32_t length;
    int32_t cnt;
    int32_t start;
    int32_t current;
    int32_t direction;
    int32_t end;
    SPFLOAT lastsamp;
    int32_t  noinsert;
} sp_waveset;

int sp_waveset_create(sp_waveset **p);
int sp_waveset_destroy(sp_waveset **p);
int sp_waveset_init(sp_data *sp, sp_waveset *p, SPFLOAT ilen);
int sp_waveset_compute(sp_data *sp, sp_waveset *p, SPFLOAT *in, SPFLOAT *out);
