typedef struct{
    void (*reinit)(void *);
    void (*compute)(void *, SPFLOAT *out);
    void *ud;
    int started;
}sp_tevent;

int sp_tevent_create(sp_tevent **te);
int sp_tevent_destroy(sp_tevent **te);
int sp_tevent_init(sp_data *sp, sp_tevent *te, 
        void (*reinit)(void*), void (*compute)(void *, SPFLOAT *out), void *ud);
int sp_tevent_compute(sp_data *sp, sp_tevent *te, SPFLOAT *in, SPFLOAT *out);
