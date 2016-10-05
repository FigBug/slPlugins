#include "soundpipe.h"
#include "md5.h"
#include "tap.h"
#include "test.h"

typedef struct {
    sp_osc *osc;
    sp_ftbl *ft;
    sp_tenv *tenv;
    sp_metro *met;
    sp_randi *rand;
    SPFLOAT freq;
} UserData;

int t_metro(sp_test *tst, sp_data *sp, const char *hash) 
{
    uint32_t n;
    int fail = 0;
    SPFLOAT osc = 0, trig = 0, tenv = 0;

    UserData ud;
    SPFLOAT *freqp = &ud.freq;
    ud.freq = 400;

    sp_srand(sp, 12345);

    sp_randi_create(&ud.rand);
    sp_metro_create(&ud.met);
    sp_tenv_create(&ud.tenv);
    sp_ftbl_create(sp, &ud.ft, 2048);
    sp_osc_create(&ud.osc);

    sp_randi_init(sp, ud.rand);
    ud.rand->min = 2.0;
    ud.rand->max= 15.0;
    sp_metro_init(sp, ud.met);
    sp_tenv_init(sp, ud.tenv);
    ud.tenv->atk = 0.005;
    ud.tenv->hold = 0.01;
    ud.tenv->rel = 0.003;
    sp_gen_sine(sp, ud.ft);
    sp_osc_init(sp, ud.osc, ud.ft, 0);
    ud.osc->freq = *freqp;

    for(n = 0; n < tst->size; n++) {
        osc = 0; trig = 0; tenv = 0;
        sp_randi_compute(sp, ud.rand, NULL, &ud.met->freq);
        sp_metro_compute(sp, ud.met, NULL, &trig);
        sp_tenv_compute(sp, ud.tenv, &trig, &tenv);
        sp_osc_compute(sp, ud.osc, NULL, &osc);
        sp_test_add_sample(tst, osc * tenv);
    }

    sp_randi_destroy(&ud.rand);
    sp_metro_destroy(&ud.met);
    sp_ftbl_destroy(&ud.ft);
    sp_osc_destroy(&ud.osc);
    sp_tenv_destroy(&ud.tenv);

    fail = sp_test_verify(tst, hash);

    if(fail) return SP_NOT_OK;
    else return SP_OK;
}
