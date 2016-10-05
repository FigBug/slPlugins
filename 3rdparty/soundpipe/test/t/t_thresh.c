#include "soundpipe.h"
#include "md5.h"
#include "tap.h"
#include "test.h"

typedef struct {
    sp_thresh *thresh;
    sp_osc *osc, *lfo;
    sp_ftbl *ft; 
    sp_tenv *tenv;
} UserData;

int t_thresh(sp_test *tst, sp_data *sp, const char *hash) 
{
    uint32_t n;
    int fail = 0;

    sp_srand(sp, 1234567);
    UserData ud;
    SPFLOAT osc = 0, thresh = 0, lfo = 0, tenv = 0;

    sp_thresh_create(&ud.thresh);
    sp_osc_create(&ud.osc);
    sp_osc_create(&ud.lfo);
    sp_tenv_create(&ud.tenv);
    sp_ftbl_create(sp, &ud.ft, 2048);

    sp_thresh_init(sp, ud.thresh);
    ud.thresh->mode = 1;
    sp_gen_sine(sp, ud.ft);
    sp_osc_init(sp, ud.osc, ud.ft, 0);

    sp_osc_init(sp, ud.lfo, ud.ft, 0);
    ud.lfo->freq = 3;
    ud.lfo->amp = 1;

    sp_tenv_init(sp, ud.tenv);
    ud.tenv->atk = 0.01;
    ud.tenv->hold = 0.01;
    ud.tenv->rel = 0.01;

    for(n = 0; n < tst->size; n++) {
        osc = 0; thresh = 0; lfo = 0; tenv = 0;
        sp_osc_compute(sp, ud.lfo, NULL, &lfo);
        sp_thresh_compute(sp, ud.thresh, &lfo, &thresh);
        sp_tenv_compute(sp, ud.tenv, &thresh, &tenv);
        sp_osc_compute(sp, ud.osc, NULL, &osc);
        sp_test_add_sample(tst, osc * tenv);
    }

    fail = sp_test_verify(tst, hash);

    sp_thresh_destroy(&ud.thresh);
    sp_ftbl_destroy(&ud.ft);
    sp_osc_destroy(&ud.osc);
    sp_osc_destroy(&ud.lfo);
    sp_tenv_destroy(&ud.tenv);

    if(fail) return SP_NOT_OK;
    else return SP_OK;
}
