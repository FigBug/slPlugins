#include "soundpipe.h"
#include "md5.h"
#include "tap.h"
#include "test.h"

typedef struct {
    sp_comb *comb;
    sp_tenv *env;
    sp_noise *nz;
} UserData; 

int t_comb(sp_test *tst, sp_data *sp, const char *hash) 
{
    sp_srand(sp, 0); 
    uint32_t n;
    int fail = 0;
    SPFLOAT tick = 0, env = 0, noise = 0, comb = 0;
    
    UserData ud;

    sp_comb_create(&ud.comb);
    sp_tenv_create(&ud.env);
    sp_noise_create(&ud.nz);

    sp_comb_init(sp, ud.comb, 0.01);
    sp_tenv_init(sp, ud.env);
    ud.env->atk = 0.001;
    ud.env->hold = 0.00;
    ud.env->rel =  0.1;

    sp_noise_init(sp, ud.nz);

    for(n = 0; n < tst->size; n++) {
        tick = 0, env = 0, noise = 0, comb = 0;

        tick = (sp->pos == 0) ? 1 : 0;
        sp_tenv_compute(sp, ud.env, &tick, &env);
        sp_noise_compute(sp, ud.nz, NULL, &noise);
        noise *= env * 0.5;
        sp_comb_compute(sp, ud.comb, &noise, &comb);
        sp_test_add_sample(tst, comb);
    }

    fail = sp_test_verify(tst, hash);
    
    sp_noise_destroy(&ud.nz);
    sp_tenv_destroy(&ud.env);
    sp_comb_destroy(&ud.comb);
     
    if(fail) return SP_NOT_OK;
    else return SP_OK;
}
