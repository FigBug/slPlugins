#include "soundpipe.h"
#include "md5.h"
#include "tap.h"
#include "test.h"

typedef struct {
    sp_pshift *pshift;
    sp_diskin *diskin;
} UserData;

int t_pshift(sp_test *tst, sp_data *sp, const char *hash) 
{
    uint32_t n;
    int fail = 0;
    sp_srand(sp, 1234567);
    UserData ud;

    SPFLOAT diskin = 0, pshift = 0;

    sp_pshift_create(&ud.pshift);
    sp_diskin_create(&ud.diskin);    

    sp_pshift_init(sp, ud.pshift);
    *ud.pshift->shift = 7;
    *ud.pshift->window = 500;
    /* half window size is smoothest sounding */
    *ud.pshift->xfade = 250;
    sp_diskin_init(sp, ud.diskin, SAMPDIR "oneart.wav");

    for(n = 0; n < tst->size; n++) {
        diskin = 0;
        pshift = 0;
        sp_diskin_compute(sp, ud.diskin, NULL, &diskin);
        sp_pshift_compute(sp, ud.pshift, &diskin, &pshift);
        sp_test_add_sample(tst, pshift);
    }

    fail = sp_test_verify(tst, hash);

    sp_pshift_destroy(&ud.pshift);
    sp_diskin_destroy(&ud.diskin);

    if(fail) return SP_NOT_OK;
    else return SP_OK;
}
