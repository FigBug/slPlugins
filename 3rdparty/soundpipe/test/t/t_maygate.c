#include "soundpipe.h"
#include "md5.h"
#include "tap.h"
#include "test.h"

typedef struct {
    sp_osc *osc;
    sp_ftbl *ft;
    sp_tevent *te;
    sp_metro *met;
    sp_tenv *tenv;
    sp_maygate *mg;
    SPFLOAT freq;
    sp_data *sp;
} UserData;

static void freq_reinit(void *ud){
    UserData *udata = ud;
    udata->osc->freq = 500 + sp_rand(udata->sp) % 2000;
}

static void freq_compute(void *ud, SPFLOAT *out){
    UserData *udata = ud;
    *out = udata->osc->freq;
}

int t_maygate(sp_test *tst, sp_data *sp, const char *hash) 
{
    uint32_t n;
    int fail = 0;
    SPFLOAT trig = 0;
    SPFLOAT env = 0;
    SPFLOAT osc = 0;
    SPFLOAT mgate = 0;
    
    sp_srand(sp, 123456);
    UserData ud;
    ud.freq = 400;
    ud.sp = sp; 
    sp_maygate_create(&ud.mg);
    sp_tenv_create(&ud.tenv);
    sp_metro_create(&ud.met);
    sp_tevent_create(&ud.te);
    sp_ftbl_create(sp, &ud.ft, 2048);
    sp_osc_create(&ud.osc);

    sp_maygate_init(sp, ud.mg);
    ud.mg->prob = 0.2;
    sp_tenv_init(sp, ud.tenv);
    ud.tenv->atk = 0.01;
    ud.tenv->hold = 0.01;
    ud.tenv->rel =  0.2;

    sp_metro_init(sp, ud.met);
    ud.met->freq = 12;
    sp_tevent_init(sp, ud.te, freq_reinit, freq_compute, &ud);
    sp_gen_sine(sp, ud.ft);
    sp_osc_init(sp, ud.osc, ud.ft, 0);


    for(n = 0; n < tst->size; n++) {
        trig = 0;
        env = 0;
        osc = 0;
        mgate = 0;
        sp_metro_compute(sp, ud.met, NULL, &trig);
        sp_maygate_compute(sp, ud.mg, &trig, &mgate);
        sp_tevent_compute(sp, ud.te, &trig, &ud.osc->freq);
        sp_tenv_compute(sp, ud.tenv, &trig, &env);
        sp_osc_compute(sp, ud.osc, NULL, &osc);
        sp->out[0] = osc * env * mgate;
        sp_test_add_sample(tst, sp->out[0]);
    }

    fail = sp_test_verify(tst, hash);

    sp_maygate_destroy(&ud.mg);
    sp_tenv_destroy(&ud.tenv);
    sp_metro_destroy(&ud.met);
    sp_ftbl_destroy(&ud.ft);
    sp_osc_destroy(&ud.osc);
    sp_tevent_destroy(&ud.te);

    if(fail) return SP_NOT_OK;
    else return SP_OK;
}