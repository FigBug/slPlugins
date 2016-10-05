typedef struct sp_tseq {
    sp_ftbl *ft;
    SPFLOAT val;
    int32_t pos;
    int shuf;
} sp_tseq;

int sp_tseq_create(sp_tseq **p);
int sp_tseq_destroy(sp_tseq **p);
int sp_tseq_init(sp_data *sp, sp_tseq *p, sp_ftbl *ft);
int sp_tseq_compute(sp_data *sp, sp_tseq *p, SPFLOAT *trig, SPFLOAT *val);
