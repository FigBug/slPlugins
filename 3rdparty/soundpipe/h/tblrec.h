typedef struct {
    sp_ftbl *ft;
    uint32_t index;
    int record;
} sp_tblrec;

int sp_tblrec_create(sp_tblrec **p);
int sp_tblrec_destroy(sp_tblrec **p);
int sp_tblrec_init(sp_data *sp, sp_tblrec *p, sp_ftbl *ft);
int sp_tblrec_compute(sp_data *sp, sp_tblrec *p, SPFLOAT *in, SPFLOAT *trig, SPFLOAT *out);
