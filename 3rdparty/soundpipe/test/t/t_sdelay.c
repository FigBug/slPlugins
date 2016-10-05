#include "soundpipe.h"
#include "md5.h"
#include "tap.h"
#include "test.h"

typedef struct {
    sp_sdelay *sdelay;
    sp_lpf18 *filt1;
    sp_lpf18 *filt2;
    sp_metro *met;
} UserData;

int t_sdelay(sp_test *tst, sp_data *sp, const char *hash) 
{
    uint32_t n;
    int fail = 0;
    SPFLOAT met = 0, sdelay = 0;
    SPFLOAT filt1 = 0, filt2 = 0;

    sp_srand(sp, 1234567);
    UserData ud;

    sp_sdelay_create(&ud.sdelay);
    sp_metro_create(&ud.met);
    sp_lpf18_create(&ud.filt1);
    sp_lpf18_create(&ud.filt2);

    sp_sdelay_init(sp, ud.sdelay, 2000);
    sp_metro_init(sp, ud.met);

    ud.met->freq = 2;

    sp_lpf18_init(sp, ud.filt1);
    ud.filt1->cutoff = 4000;
    ud.filt1->res = 0.8;

    sp_lpf18_init(sp, ud.filt2);
    ud.filt2->cutoff = 500;
    ud.filt2->res = 0.8;

    for(n = 0; n < tst->size; n++) {
        met = 0;
        sdelay = 0;
        filt1 = 0; 
        filt2 = 0;
        sp_metro_compute(sp, ud.met, NULL, &met);
        sp_sdelay_compute(sp, ud.sdelay, &met, &sdelay);

        sp_lpf18_compute(sp, ud.filt1, &met, &filt1);
        sp_lpf18_compute(sp, ud.filt2, &sdelay, &filt2);

        sp->out[0] = filt1 + filt2;
        sp_test_add_sample(tst, sp->out[0]);
    }

    fail = sp_test_verify(tst, hash);

    sp_sdelay_destroy(&ud.sdelay);
    sp_metro_destroy(&ud.met);
    sp_lpf18_destroy(&ud.filt1);
    sp_lpf18_destroy(&ud.filt2);

    if(fail) return SP_NOT_OK;
    /* fail by default */
    else return SP_OK;
}
