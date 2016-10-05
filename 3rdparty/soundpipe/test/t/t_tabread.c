#include "soundpipe.h"
#include "md5.h"
#include "tap.h"
#include "test.h"

typedef struct {
    sp_tabread *tr;
    sp_ftbl *ft;
    sp_phasor *phasor;
} UserData;

int t_tabread(sp_test *tst, sp_data *sp, const char *hash) 
{
    uint32_t n;
    int fail = 0;
    SPFLOAT tab = 0.0, phasor = 0.0;

    sp_srand(sp, 123456);
    UserData ud;

    sp_tabread_create(&ud.tr);

    sp_phasor_create(&ud.phasor);

    sp_ftbl_create(sp, &ud.ft, 395393);
    sp_gen_file(sp, ud.ft, "oneart.wav");

    sp_tabread_init(sp, ud.tr, ud.ft, 1);

    /* since mode = 1, offset 5% into file */
    ud.tr->offset = 0.05;
    /* no wraparound */
    ud.tr->wrap = 0;

    sp_phasor_init(sp, ud.phasor, 0);
    /* set playback rate to half speed, or 1/(t * 2) */
    ud.phasor->freq = 1 / (8.97 * 2);
    sp->len = 44100 * 5;


    for(n = 0; n < tst->size; n++) {
        tab = 0.0; phasor = 0.0;
        sp_phasor_compute(sp, ud.phasor, NULL, &phasor);
        ud.tr->index = phasor;
        sp_tabread_compute(sp, ud.tr, NULL, &tab);
        sp_test_add_sample(tst, 0);
    }

    fail = sp_test_verify(tst, hash);

    sp_phasor_destroy(&ud.phasor);
    sp_tabread_destroy(&ud.tr);
    sp_ftbl_destroy(&ud.ft);

    if(fail) return SP_NOT_OK;
    else return SP_OK;
}
