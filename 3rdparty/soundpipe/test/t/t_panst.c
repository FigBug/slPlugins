#include "soundpipe.h"
#include "md5.h"
#include "tap.h"
#include "test.h"

typedef struct {
    sp_panst *panst;
    sp_osc *osc;
    sp_osc *lfo;
    sp_ftbl *ft;
} UserData;

int t_panst(sp_test *tst, sp_data *sp, const char *hash) 
{
    uint32_t n;
    int fail = 0;

    UserData ud;
    SPFLOAT osc = 0, outL = 0, outR = 0, lfo = 0;

    sp_panst_create(&ud.panst);
    sp_osc_create(&ud.osc);
    sp_osc_create(&ud.lfo);
    sp_ftbl_create(sp, &ud.ft, 2048);

    sp_panst_init(sp, ud.panst);
    ud.panst->type = 0;
    sp_gen_sine(sp, ud.ft);
    sp_osc_init(sp, ud.osc, ud.ft, 0);
    sp_osc_init(sp, ud.lfo, ud.ft, 0);
    ud.lfo->amp = 1;
    ud.lfo->freq = 0.5;

    sp->len = 44100 * 5;

    for(n = 0; n < tst->size; n += 2) {
        osc = 0; outL = 0; outR = 0; lfo = 0;

        sp_osc_compute(sp, ud.osc, NULL, &osc);
        sp_osc_compute(sp, ud.lfo, NULL, &lfo);
        ud.panst->pan = lfo;

        sp_panst_compute(sp, ud.panst, &osc, &osc, &outL, &outR);
        sp_test_add_sample(tst, outL);
        sp_test_add_sample(tst, outR);
    }

    fail = sp_test_verify(tst, hash);

    sp_panst_destroy(&ud.panst);
    sp_ftbl_destroy(&ud.ft);
    sp_osc_destroy(&ud.osc);
    sp_osc_destroy(&ud.lfo);


    if(fail) return SP_NOT_OK;
    else return SP_OK;
}
