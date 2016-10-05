#include "soundpipe.h"
#include "md5.h"
#include "tap.h"
#include "test.h"

typedef struct {
    sp_osc *osc;
    sp_ftbl *ft, *delta;
    sp_tenv *tenv;
    sp_dtrig *dt;
} UserData;

int t_dtrig(sp_test *tst, sp_data *sp, const char *hash) 
{
    uint32_t n;
    int fail = 0;
    UserData ud;

    SPFLOAT env = 0;
    SPFLOAT osc = 0;
    SPFLOAT trig;
    SPFLOAT dtrig = 0;

    sp_dtrig_create(&ud.dt);
    sp_tenv_create(&ud.tenv);
    sp_ftbl_create(sp, &ud.ft, 2048);
    sp_ftbl_create(sp, &ud.delta, 4);
    sp_osc_create(&ud.osc);

    ud.delta->tbl[0] = 1.0;
    ud.delta->tbl[1] = 1.0;
    ud.delta->tbl[2] = 0.5;
    ud.delta->tbl[3] = 0.5;
    sp_dtrig_init(sp, ud.dt, ud.delta);
    sp_tenv_init(sp, ud.tenv);
    ud.tenv->atk = 0.03;
    ud.tenv->hold = 0.01;
    ud.tenv->rel =  0.3;
    sp_gen_sine(sp, ud.ft);
    sp_osc_init(sp, ud.osc, ud.ft, 0);
    ud.osc->freq = 1000;
    ud.osc->amp = 0.5;

    for(n = 0; n < tst->size; n++) {
        env = 0;
        osc = 0;
        trig = 0;
        dtrig = 0;
        if(sp->pos == 0){
            trig = 1.0;
        }else{
            trig = 0.0;
        }
        sp_dtrig_compute(sp, ud.dt, &trig, &dtrig);
        sp_tenv_compute(sp, ud.tenv, &dtrig, &env);
        sp_osc_compute(sp, ud.osc, NULL, &osc);
        sp_test_add_sample(tst, osc * env);
    }
    
    fail = sp_test_verify(tst, hash);
     
    sp_dtrig_destroy(&ud.dt);
    sp_tenv_destroy(&ud.tenv);
    sp_ftbl_destroy(&ud.ft);
    sp_ftbl_destroy(&ud.delta);
    sp_osc_destroy(&ud.osc);

    if(fail) return SP_NOT_OK;
    else return SP_OK;
}
