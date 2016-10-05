#include "soundpipe.h"
#include "md5.h"
#include "tap.h"
#include "test.h"

typedef struct {
    sp_biscale *biscale;
    sp_osc *osc;
    sp_ftbl *ft;
} UserData;

int t_biscale(sp_test *tst, sp_data *sp, const char *hash)
{
    uint32_t n;
    int fail = 0;
    SPFLOAT val = 1;
    SPFLOAT osc = 0, biscale = 0;

    sp_srand(sp, 1234567);
    UserData ud;

    sp_biscale_create(&ud.biscale);
    sp_osc_create(&ud.osc);
    sp_ftbl_create(sp, &ud.ft, 2048);

    sp_biscale_init(sp, ud.biscale);
    ud.biscale->min = 0;
    ud.biscale->max = 880;
    sp_gen_sine(sp, ud.ft);
    sp_osc_init(sp, ud.osc, ud.ft, 0);
    ud.osc->amp = 0.1;

    for(n = 0; n < tst->size; n++) {
        osc = 0, biscale = 0;
        /* constant set to 1, when biscaled, it becomes 440 */
        val = 1;
        sp_biscale_compute(sp, ud.biscale, &val, &biscale);
        ud.osc->freq = biscale;
        sp_osc_compute(sp, ud.osc, NULL, &osc);
        sp->out[0] = osc;
        sp_test_add_sample(tst, sp->out[0]);
    }

    fail = sp_test_verify(tst, hash);

    sp_biscale_destroy(&ud.biscale);
    sp_ftbl_destroy(&ud.ft);
    sp_osc_destroy(&ud.osc);

    if(fail) return SP_NOT_OK;
    else return SP_OK;
}
