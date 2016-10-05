#include "soundpipe.h"
#include "md5.h"
#include "tap.h"
#include "test.h"

typedef struct {
    sp_prop *prop;
    sp_osc *osc;
    sp_ftbl *ft;
    sp_tenv *tenv;
} UserData;

int t_prop(sp_test *tst, sp_data *sp, const char *hash) 
{
    uint32_t n;
    int fail = 0;
    SPFLOAT osc = 0, prop = 0, tenv = 0;
    
    sp_srand(sp, 1234567);
    UserData ud;
    
    sp_prop_create(&ud.prop);
    sp_osc_create(&ud.osc);
    sp_ftbl_create(sp, &ud.ft, 2048);
    sp_tenv_create(&ud.tenv);

    sp_prop_init(sp, ud.prop, "2(++)3(+++)-2(-2(++))+5(+++++)");
    ud.prop->bpm = 80;
    sp_gen_sine(sp, ud.ft);
    sp_osc_init(sp, ud.osc, ud.ft, 0);
    sp_tenv_init(sp, ud.tenv);
    ud.tenv->atk = 0.01;
    ud.tenv->hold = 0.01;
    ud.tenv->rel =  0.2;

    ud.osc->freq = 500;

    for(n = 0; n < tst->size; n++) {

        osc = 0, prop = 0, tenv = 0;
        ud.prop->bpm = 80;
        sp_osc_compute(sp, ud.osc, NULL, &osc);
        sp_prop_compute(sp, ud.prop, NULL, &prop);
        sp_tenv_compute(sp, ud.tenv, &prop, &tenv);
        sp->out[0] = osc * tenv;
        sp_test_add_sample(tst, sp->out[0]);
    }

    fail = sp_test_verify(tst, hash);
    
    sp_prop_destroy(&ud.prop);
    sp_ftbl_destroy(&ud.ft);
    sp_osc_destroy(&ud.osc);
    sp_tenv_destroy(&ud.tenv);

    if(fail) return SP_NOT_OK;
    else return SP_OK;
}
