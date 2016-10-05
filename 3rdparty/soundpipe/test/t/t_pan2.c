#include "soundpipe.h"
#include "md5.h"
#include "tap.h"
#include "test.h"

typedef struct {
    sp_pan2 *pan2;
    sp_osc *osc;
    sp_osc *lfo;
    sp_ftbl *ft; 
} UserData;

int t_pan2(sp_test *tst, sp_data *sp, const char *hash) 
{
    uint32_t n;
    int fail = 0;
    SPFLOAT osc = 0, outL = 0, outR = 0, lfo = 0;

    UserData ud;

    sp_pan2_create(&ud.pan2);
    sp_osc_create(&ud.osc);
    sp_osc_create(&ud.lfo);
    sp_ftbl_create(sp, &ud.ft, 2048);

    sp_pan2_init(sp, ud.pan2);
    ud.pan2->type = 2;
    sp_gen_sine(sp, ud.ft);
    sp_osc_init(sp, ud.osc, ud.ft, 0);
    sp_osc_init(sp, ud.lfo, ud.ft, 0);
    ud.lfo->amp = 1;
    ud.lfo->freq = 1;

    for(n = 0; n < tst->size; n++) {
        osc = 0; outL = 0; outR = 0; lfo = 0;
        sp_osc_compute(sp, ud.osc, NULL, &osc);
        sp_osc_compute(sp, ud.lfo, NULL, &lfo);
        ud.pan2->pan = lfo;
        sp_pan2_compute(sp, ud.pan2, &osc, &outL, &outR);
        sp_test_add_sample(tst, outL);
    }

    fail = sp_test_verify(tst, hash);

    sp_pan2_destroy(&ud.pan2);
    sp_ftbl_destroy(&ud.ft);
    sp_osc_destroy(&ud.osc);
    sp_osc_destroy(&ud.lfo);

    if(fail) return SP_NOT_OK;
    else return SP_OK;
}
