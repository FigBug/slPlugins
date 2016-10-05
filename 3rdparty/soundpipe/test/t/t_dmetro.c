#include "soundpipe.h"
#include "md5.h"
#include "tap.h"
#include "test.h"

typedef struct {
    sp_dmetro *dmetro;
    sp_osc *osc;
    sp_osc *lfo;
    sp_ftbl *ft;
    sp_tenv *tenv;
} UserData;

int t_dmetro(sp_test *tst, sp_data *sp, const char *hash) 
{
    uint32_t n;
    int fail = 0;
    UserData ud;
    SPFLOAT osc = 0, dmetro = 0, tenv = 0, lfo = 0;
    
    sp_dmetro_create(&ud.dmetro);
    sp_osc_create(&ud.osc);
    sp_osc_create(&ud.lfo);
    sp_ftbl_create(sp, &ud.ft, 2048);
    sp_tenv_create(&ud.tenv);

    sp_dmetro_init(sp, ud.dmetro);
    ud.dmetro->time = 0.05;
    sp_gen_sine(sp, ud.ft);
    sp_osc_init(sp, ud.osc, ud.ft, 0);
    sp_osc_init(sp, ud.lfo, ud.ft, 0);
    ud.lfo->freq = 0.3;
    ud.lfo->amp = 0.5;
    sp_tenv_init(sp, ud.tenv);
    ud.tenv->atk = 0.001;
    ud.tenv->hold = 0.03;
    ud.tenv->rel =  0.001;

    for(n = 0; n < tst->size; n++) {
        osc = 0, dmetro = 0, tenv = 0, lfo = 0;
        sp_osc_compute(sp, ud.osc, NULL, &osc);
        sp_osc_compute(sp, ud.lfo, NULL, &lfo);
        lfo += 0.5;
        ud.dmetro->time = 0.05 + 0.3 * lfo;
        sp_dmetro_compute(sp, ud.dmetro, NULL, &dmetro);
        sp_tenv_compute(sp, ud.tenv, &dmetro, &tenv);
        sp_test_add_sample(tst, tenv * osc);
    }

    fail = sp_test_verify(tst, hash);
    
    sp_dmetro_destroy(&ud.dmetro);
    sp_ftbl_destroy(&ud.ft);
    sp_osc_destroy(&ud.osc);
    sp_osc_destroy(&ud.lfo);
    sp_tenv_destroy(&ud.tenv);
     
    if(fail) return SP_NOT_OK;
    else return SP_OK;
}
