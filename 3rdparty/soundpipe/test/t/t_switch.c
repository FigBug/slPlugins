#include "soundpipe.h"
#include "md5.h"
#include "tap.h"
#include "test.h"

typedef struct {
    sp_switch *sw;
    sp_osc *osc;
    sp_ftbl *ft;
    sp_metro *met;
    sp_osc *lfo;
    sp_fosc *fosc;
} UserData;

int t_switch(sp_test *tst, sp_data *sp, const char *hash) 
{
    uint32_t n;
    int fail = 0;
    SPFLOAT osc = 0, sw = 0, met = 0, fosc = 0, lfo = 0;

    sp_srand(sp, 1234567);
    UserData ud;

    sp_switch_create(&ud.sw);
    sp_osc_create(&ud.osc);
    sp_osc_create(&ud.lfo);
    sp_ftbl_create(sp, &ud.ft, 2048);
    sp_metro_create(&ud.met);
    sp_fosc_create(&ud.fosc);

    sp_switch_init(sp, ud.sw);
    sp_gen_sine(sp, ud.ft);
    sp_osc_init(sp, ud.osc, ud.ft, 0);
    sp_osc_init(sp, ud.lfo, ud.ft, 0);
    ud.lfo->amp = 100;
    ud.lfo->freq = 6;
    sp_fosc_init(sp, ud.fosc, ud.ft);
    sp_metro_init(sp, ud.met);
    ud.met->freq = 2.5;

    for(n = 0; n < tst->size; n++) {
        osc = 0; sw = 0; met = 0; fosc = 0; lfo = 0;
        sp_osc_compute(sp, ud.lfo, NULL, &lfo);
        ud.osc->freq = 550 + lfo;
        sp_osc_compute(sp, ud.osc, NULL, &osc);
        sp_fosc_compute(sp, ud.fosc, NULL, &fosc);
        sp_metro_compute(sp, ud.met, NULL, &met);
        sp_switch_compute(sp, ud.sw, &met, &osc, &fosc, &sw);
        sp_test_add_sample(tst, sw);
    }

    fail = sp_test_verify(tst, hash);

    sp_switch_destroy(&ud.sw);
    sp_ftbl_destroy(&ud.ft);
    sp_osc_destroy(&ud.osc);
    sp_osc_destroy(&ud.lfo);
    sp_fosc_destroy(&ud.fosc);
    sp_metro_destroy(&ud.met);

    if(fail) return SP_NOT_OK;
    else return SP_OK;
}
