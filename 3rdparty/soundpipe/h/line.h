typedef struct {
    SPFLOAT a, dur, b;
    SPFLOAT val, incr; 
    uint32_t sdur, stime;
    int init;
} sp_line;

int sp_line_create(sp_line **p);
int sp_line_destroy(sp_line **p);
int sp_line_init(sp_data *sp, sp_line *p);
int sp_line_compute(sp_data *sp, sp_line *p, SPFLOAT *in, SPFLOAT *out);
