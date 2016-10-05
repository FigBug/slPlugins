typedef struct{
    SPFLOAT htime;
    SPFLOAT c1, c2, yt1, prvhtim;
    SPFLOAT sr, onedsr;
}sp_port;

int sp_port_create(sp_port **p);
int sp_port_destroy(sp_port **p);
int sp_port_init(sp_data *sp, sp_port *p, SPFLOAT htime);
int sp_port_compute(sp_data *sp, sp_port *p, SPFLOAT *in, SPFLOAT *out);
