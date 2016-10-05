#include "soundpipe.h"
#include "md5.h"
#include "tap.h"
#include "test.h"

typedef struct {
    sp_wpkorg35 *wpkorg35;
    sp_noise *noise;
    sp_randi *randi;
} UserData;

int t_wpkorg35(sp_test *tst, sp_data *sp, const char *hash) 
{
    uint32_t n;
    int fail = 0;
    SPFLOAT noise = 0, wpkorg35 = 0, randi = 0;

    sp_srand(sp, 1234567);
    UserData ud;

    sp_wpkorg35_create(&ud.wpkorg35);
    sp_noise_create(&ud.noise);
    sp_randi_create(&ud.randi);

    sp_wpkorg35_init(sp, ud.wpkorg35);
    ud.wpkorg35->res = 1.99;
    sp_noise_init(sp, ud.noise);
    ud.noise->amp = 0.01;
    sp_randi_init(sp, ud.randi);
    ud.randi->min = 100;
    ud.randi->max = 3000;
    ud.randi->cps = 10;

    for(n = 0; n < tst->size; n++) {
        noise = 0; 
        wpkorg35 = 0; 
        randi = 0;
        sp_randi_compute(sp, ud.randi, NULL, &randi);
        sp_noise_compute(sp, ud.noise, NULL, &noise);
        ud.wpkorg35->cutoff = randi;
        sp_wpkorg35_compute(sp, ud.wpkorg35, &noise, &wpkorg35);
        sp_test_add_sample(tst, wpkorg35);
    }

    fail = sp_test_verify(tst, hash);

    sp_wpkorg35_destroy(&ud.wpkorg35);
    sp_noise_destroy(&ud.noise);
    sp_randi_destroy(&ud.randi);

    if(fail) return SP_NOT_OK;
    else return SP_OK;
}
