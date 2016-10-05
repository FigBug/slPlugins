typedef struct {
    SPFLOAT sig;
    SPFLOAT index, mode, offset, wrap;
    SPFLOAT mul;
    sp_ftbl *ft;
} sp_tabread;

int sp_tabread_create(sp_tabread **p);
int sp_tabread_destroy(sp_tabread **p);
int sp_tabread_init(sp_data *sp, sp_tabread *p, sp_ftbl *ft, int mode);
int sp_tabread_compute(sp_data *sp, sp_tabread *p, SPFLOAT *in, SPFLOAT *out);
