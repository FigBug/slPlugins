#include "soundpipe.h"
#include "md5.h"
#include "tap.h"
#include "test.h"

typedef struct {
    sp_tenv2 *tenv2;
    sp_osc *osc;
    sp_ftbl *ft; 
} UserData;

int t_tenv2(sp_test *tst, sp_data *sp, const char *hash) 
{
    uint32_t n;
    int fail = 0;
    sp_srand(sp, 1234567);
    UserData ud;
    SPFLOAT osc = 0, tenv2 = 0, tick = 0;

    sp_tenv2_create(&ud.tenv2);
    sp_osc_create(&ud.osc);
    sp_ftbl_create(sp, &ud.ft, 2048);

    sp_tenv2_init(sp, ud.tenv2);
    ud.tenv2->atk = 3;
    ud.tenv2->rel = 1;
    sp_gen_sine(sp, ud.ft);
    sp_osc_init(sp, ud.osc, ud.ft, 0);
    ud.osc->amp = 0.6;

    for(n = 0; n < tst->size; n++) {
        osc = 0, tenv2 = 0, tick = 0;

        if(sp->pos == 0 || sp->pos == sp->sr * 1) tick = 1;

        sp_osc_compute(sp, ud.osc, NULL, &osc);
        sp_tenv2_compute(sp, ud.tenv2, &tick, &tenv2);
        sp->out[0] = osc * tenv2;
        sp_test_add_sample(tst, sp->out[0]);
    }

    fail = sp_test_verify(tst, hash);
    
    sp_tenv2_destroy(&ud.tenv2);
    sp_ftbl_destroy(&ud.ft);
    sp_osc_destroy(&ud.osc);

    if(fail) return SP_NOT_OK;
    else return SP_OK;
}
