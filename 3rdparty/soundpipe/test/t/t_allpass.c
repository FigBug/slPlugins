#include "soundpipe.h"
#include "md5.h"
#include "tap.h"
#include "test.h"

int t_allpass(sp_test *tst, sp_data *sp, const char *hash) 
{
    sp_srand(sp, 0); 
    uint32_t n;
    int fail = 0;
    sp_allpass *ap_d;
    sp_tenv *env_d;
    sp_noise *nz_d;

    SPFLOAT tick = 0, env = 0, noise = 0, allpass = 0;

    sp_allpass_create(&ap_d);
    sp_tenv_create(&env_d);
    sp_noise_create(&nz_d);

    sp_allpass_init(sp, ap_d, 0.1);
    sp_tenv_init(sp, env_d);
    env_d->atk = 0.001;
    env_d->hold = 0.00;
    env_d->rel =  0.1;

    sp_noise_init(sp, nz_d);

    for(n = 0; n < tst->size; n++) {

        tick = 0, env = 0, noise = 0, allpass = 0;
        tick = (n == 0) ? 1 : 0;
        sp_tenv_compute(sp, env_d, &tick, &env);
        sp_noise_compute(sp, nz_d, NULL, &noise);
        noise *= env * 0.5;
        sp_allpass_compute(sp, ap_d, &noise, &allpass);

        sp_test_add_sample(tst, allpass);
    }

    fail = sp_test_verify(tst,hash);

    sp_noise_destroy(&nz_d);
    sp_tenv_destroy(&env_d);
    sp_allpass_destroy(&ap_d);
     
    if(fail) return SP_NOT_OK;
    else return SP_OK;
}
