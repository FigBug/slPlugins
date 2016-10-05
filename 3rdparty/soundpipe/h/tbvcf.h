typedef struct{
    SPFLOAT fco, res, dist, asym, iskip, y, y1, y2;
    int fcocod, rezcod;
    SPFLOAT sr;
    SPFLOAT onedsr;

}sp_tbvcf;

int sp_tbvcf_create(sp_tbvcf **p);
int sp_tbvcf_destroy(sp_tbvcf **p);
int sp_tbvcf_init(sp_data *sp, sp_tbvcf *p);
int sp_tbvcf_compute(sp_data *sp, sp_tbvcf *p, SPFLOAT *in, SPFLOAT *out);
