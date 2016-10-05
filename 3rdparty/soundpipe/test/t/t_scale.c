#include "soundpipe.h"
#include "md5.h"
#include "tap.h"
#include "test.h"

typedef struct {
    sp_scale *scale;
    sp_osc *osc;
    sp_ftbl *ft;
} UserData;

int t_scale(sp_test *tst, sp_data *sp, const char *hash)
{
    uint32_t n;
    int fail = 0;
    SPFLOAT val = 1;
    SPFLOAT osc = 0, scale = 0;

    sp_srand(sp, 1234567);
    UserData ud;

    sp_scale_create(&ud.scale);
    sp_osc_create(&ud.osc);
    sp_ftbl_create(sp, &ud.ft, 2048);

    sp_scale_init(sp, ud.scale);
    ud.scale->min = 0;
    ud.scale->max = 880;
    sp_gen_sine(sp, ud.ft);
    sp_osc_init(sp, ud.osc, ud.ft, 0);
    ud.osc->amp = 0.1;

    for(n = 0; n < tst->size; n++) {
        osc = 0, scale = 0;
        /* constant set to 1, when scaled, it becomes 440 */
        val = 1;
        sp_scale_compute(sp, ud.scale, &val, &scale);
        ud.osc->freq = scale;
        sp_osc_compute(sp, ud.osc, NULL, &osc);
        sp->out[0] = osc;
        sp_test_add_sample(tst, sp->out[0]);
    }

    fail = sp_test_verify(tst, hash);

    sp_scale_destroy(&ud.scale);
    sp_ftbl_destroy(&ud.ft);
    sp_osc_destroy(&ud.osc);

    if(fail) return SP_NOT_OK;
    else return SP_OK;
}
