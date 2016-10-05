#include "soundpipe.h"
#include "md5.h"
#include "tap.h"
#include "test.h"

typedef struct {
    sp_hilbert *hilbert;
    sp_osc *cos, *sin;
    sp_ftbl *ft; 
    sp_diskin *diskin;
} UserData;

int t_hilbert(sp_test *tst, sp_data *sp, const char *hash) 
{
    uint32_t n;
    int fail = 0;
    SPFLOAT real = 0, imag = 0;
    SPFLOAT diskin = 0;
    SPFLOAT sin = 0, cos = 0;

    sp_srand(sp, 1234567);
    UserData ud;

    sp_hilbert_create(&ud.hilbert);
    sp_osc_create(&ud.sin);
    sp_osc_create(&ud.cos);
    sp_diskin_create(&ud.diskin);
    sp_ftbl_create(sp, &ud.ft, 8192);

    sp_hilbert_init(sp, ud.hilbert);
    sp_gen_sine(sp, ud.ft);
    sp_osc_init(sp, ud.sin, ud.ft, 0);
    sp_osc_init(sp, ud.cos, ud.ft, 0.25);
    ud.sin->freq = 1000;
    ud.cos->freq = 1000;
    sp_diskin_init(sp, ud.diskin, SAMPDIR "oneart.wav");

    for(n = 0; n < tst->size; n++) {
        real = 0; 
        imag = 0;
        diskin = 0;
        sin = 0; 
        cos = 0;

        sp_diskin_compute(sp, ud.diskin, NULL, &diskin);
        sp_osc_compute(sp, ud.sin, NULL, &sin);
        sp_osc_compute(sp, ud.cos, NULL, &cos);
        sp_hilbert_compute(sp, ud.hilbert, &diskin, &real, &imag);
        sp->out[0] = ((cos * real) + (sin * real)) * 0.7;
        sp_test_add_sample(tst, sp->out[0]);
    }

    fail = sp_test_verify(tst, hash);

    sp_hilbert_destroy(&ud.hilbert);
    sp_ftbl_destroy(&ud.ft);
    sp_osc_destroy(&ud.sin);
    sp_osc_destroy(&ud.cos);
    sp_diskin_destroy(&ud.diskin);

    if(fail) return SP_NOT_OK;
    else return SP_OK;
}
