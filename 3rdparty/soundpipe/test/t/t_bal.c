#include "soundpipe.h"
#include "md5.h"
#include "tap.h"
#include "test.h"

int t_bal(sp_test *tst, sp_data *sp, const char *hash) 
{
    uint32_t n, i;
    int fail = 0;
    SPFLOAT out = 0, osc = 0, filt = 0, bal = 0, env = 0, tick;
    
    sp_osc *osc_d[3];
    sp_ftbl *ft_d;
    sp_moogladder *filt_d;
    sp_bal *bal_d;
    sp_tenv *env_d;
    
    sp_ftbl_create(sp, &ft_d, 4096);
    for(i = 0; i < 3; i++) {
        sp_osc_create(&osc_d[i]);
        osc_d[i]->amp = 0.5;
    }
    sp_bal_create(&bal_d);
    sp_moogladder_create(&filt_d);
    filt_d->res = 0.8;
    sp_tenv_create(&env_d);

    sp_gen_line(sp, ft_d, "0 1 4096 -1");
    sp_osc_init(sp, osc_d[0], ft_d, 0);
    osc_d[0]->freq = sp_midi2cps(41 - 0.05);
    sp_osc_init(sp, osc_d[1], ft_d, 0);
    osc_d[1]->freq = sp_midi2cps(41 - 12);
    sp_osc_init(sp, osc_d[2], ft_d, 0);
    osc_d[2]->freq = sp_midi2cps(41 + 0.05);
    sp->len = 44100 * 5;
    sp_bal_init(sp, bal_d);
    sp_moogladder_init(sp, filt_d);
    sp_tenv_init(sp, env_d);
    env_d->atk = 2.25;
    env_d->hold = 0.5;
    env_d->rel =  2.25;


    for(n = 0; n < tst->size; n++) {
        out = 0, osc = 0, filt = 0, bal = 0, env = 0;
        for(i = 0; i < 3; i++) {
           sp_osc_compute(sp, osc_d[i], NULL, &osc);
           out += osc * 0.5;
        }
        tick = (sp->pos == 0) ? 1.0 : 0.0;
        sp_tenv_compute(sp, env_d, &tick, &env);
        filt_d->freq = 300 + 3000 * env;
        sp_moogladder_compute(sp, filt_d, &out, &filt);
        sp_bal_compute(sp, bal_d, &filt, &osc, &bal);
        sp_test_add_sample(tst, bal * env);
    }

    fail = sp_test_verify(tst, hash);
    
    sp_ftbl_destroy(&ft_d);
    for(i = 0; i < 3; i++) sp_osc_destroy(&osc_d[i]);
    sp_bal_destroy(&bal_d);
    sp_moogladder_destroy(&filt_d);
    sp_tenv_destroy(&env_d);
     
    if(fail) return SP_NOT_OK;
    else return SP_OK;
}
