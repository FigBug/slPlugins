#include "soundpipe.h"
#include "md5.h"
#include "tap.h"
#include "test.h"

typedef struct {
    sp_osc *osc;
    sp_ftbl *ft;
    sp_tenv *tenv;
    sp_reverse *rv;
} UserData;

int t_reverse(sp_test *tst, sp_data *sp, const char *hash) 
{
    uint32_t n;
    int fail = 0;
    sp_srand(sp, 123456);
    UserData ud; 
    SPFLOAT env = 0;
    SPFLOAT osc = 0;
    SPFLOAT rv = 0;
    SPFLOAT dry = 0;
    SPFLOAT trig;
    
    sp_reverse_create(&ud.rv);
    sp_tenv_create(&ud.tenv);
    sp_ftbl_create(sp, &ud.ft, 2048);
    sp_osc_create(&ud.osc);

    sp_reverse_init(sp, ud.rv, 1.0);
    sp_tenv_init(sp, ud.tenv);
    ud.tenv->atk = 0.03;
    ud.tenv->hold = 0.01;
    ud.tenv->rel =  0.3;

    sp_gen_sine(sp, ud.ft);
    sp_osc_init(sp, ud.osc, ud.ft, 0);
    ud.osc->freq = 1000;
    ud.osc->amp = 0.5;

    /* allocate / initialize modules here */

    for(n = 0; n < tst->size; n++) {
        env = 0;
        osc = 0;
        rv = 0;
        dry = 0;
        trig = 0;

        if(n == 0){
            trig = 1.0;
        }else{
            trig = 0.0;
        }
        sp_tenv_compute(sp, ud.tenv, &trig, &env);
        sp_osc_compute(sp, ud.osc, NULL, &osc);
        dry = osc * env;
        sp_reverse_compute(sp, ud.rv, &dry, &rv);
        sp->out[0] = dry + rv;
        sp_test_add_sample(tst, sp->out[0]);
    }

    fail = sp_test_verify(tst, hash);

    /* destroy functions here */
    
    sp_reverse_destroy(&ud.rv);
    sp_tenv_destroy(&ud.tenv);
    sp_ftbl_destroy(&ud.ft);
    sp_osc_destroy(&ud.osc);

    if(fail) return SP_NOT_OK;
    else return SP_OK;
}
