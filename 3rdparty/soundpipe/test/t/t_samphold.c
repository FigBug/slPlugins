#include "soundpipe.h"
#include "md5.h"
#include "tap.h"
#include "test.h"

typedef struct {
    sp_samphold *samphold;
    sp_osc *osc;
    sp_ftbl *ft;
    sp_metro *met;
    sp_noise *noise;
} UserData;

int t_samphold(sp_test *tst, sp_data *sp, const char *hash) 
{
    sp_srand(sp, 1234567);
    UserData ud;
    uint32_t n;
    int fail = 0;
    SPFLOAT osc = 0, samphold = 0, met = 0, noise = 0;

    sp_samphold_create(&ud.samphold);
    sp_osc_create(&ud.osc);
    sp_ftbl_create(sp, &ud.ft, 2048);
    sp_metro_create(&ud.met);
    sp_noise_create(&ud.noise);

    sp_samphold_init(sp, ud.samphold);
    sp_gen_sine(sp, ud.ft);
    sp_osc_init(sp, ud.osc, ud.ft, 0);
    sp_metro_init(sp, ud.met);
    ud.met->freq = 5;
    sp_noise_init(sp, ud.noise);
    ud.noise->amp = 1;
    sp->len = 44100 * 5;


    for(n = 0; n < tst->size; n++) {
        osc = 0, samphold = 0, met = 0, noise = 0;
        sp_metro_compute(sp, ud.met, NULL, &met);
        sp_noise_compute(sp, ud.noise, NULL, &noise);
        sp_samphold_compute(sp, ud.samphold, &met, &noise, &samphold);
        ud.osc->freq = 200 + (samphold + 1) * 300;
        sp_osc_compute(sp, ud.osc, NULL, &osc);
        sp->out[0] = osc;
        sp_test_add_sample(tst, sp->out[0]);
    }

    fail = sp_test_verify(tst, hash);

    sp_samphold_destroy(&ud.samphold);
    sp_ftbl_destroy(&ud.ft);
    sp_osc_destroy(&ud.osc);
    sp_metro_destroy(&ud.met);
    sp_noise_destroy(&ud.noise);

    if(fail) return SP_NOT_OK;
    else return SP_OK;
}
