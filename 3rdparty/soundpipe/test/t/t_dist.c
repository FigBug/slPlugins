#include "soundpipe.h"
#include "md5.h"
#include "tap.h"
#include "test.h"

typedef struct {
    sp_fosc *osc;
    sp_ftbl *ft;
    sp_dist *ds;
    sp_osc *lfo;
} UserData;

int t_dist(sp_test *tst, sp_data *sp, const char *hash) 
{
    uint32_t n;
    int fail = 0;
    SPFLOAT osc = 0, lfo = 0, out = 0;
    UserData ud;
    
    sp_ftbl_create(sp, &ud.ft, 2048);
    sp_fosc_create(&ud.osc);
    sp_dist_create(&ud.ds);
    sp_osc_create(&ud.lfo);

    sp_gen_sine(sp, ud.ft);
    sp_fosc_init(sp, ud.osc, ud.ft);
    ud.osc->freq = 60;
    sp_dist_init(sp, ud.ds);
    ud.ds->pregain = 10;
    sp_osc_init(sp, ud.lfo, ud.ft, 0);
    ud.lfo->freq = 0.5;


    for(n = 0; n < tst->size; n++) {
        osc = 0, lfo = 0, out = 0;
        sp_osc_compute(sp, ud.lfo, NULL, &lfo);
        lfo = 7 * (0.5 * (lfo + 1));
        ud.osc->indx = lfo;
        sp_fosc_compute(sp, ud.osc, NULL, &osc);
        sp_dist_compute(sp, ud.ds, &osc, &out);

        sp_test_add_sample(tst, out);
    }

    fail = sp_test_verify(tst, hash);
     
    sp_ftbl_destroy(&ud.ft);
    sp_fosc_destroy(&ud.osc);
    sp_dist_destroy(&ud.ds);
    sp_osc_destroy(&ud.lfo);

    if(fail) return SP_NOT_OK;
    else return SP_OK;
}
