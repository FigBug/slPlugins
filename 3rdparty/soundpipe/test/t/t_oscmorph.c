#include "soundpipe.h"
#include "md5.h"
#include "tap.h"
#include "test.h"

typedef struct {
    sp_oscmorph *oscmorph;
    sp_ftbl *wt1;
    sp_osc *osc;
    sp_ftbl *ft; 
} UserData;

int t_oscmorph(sp_test *tst, sp_data *sp, const char *hash) 
{
    uint32_t n;
    int fail = 0;
    SPFLOAT osc = 0, oscmorph = 0;

    sp_srand(sp, 1234567);
    UserData ud;

    sp_oscmorph_create(&ud.oscmorph);
    sp_osc_create(&ud.osc);
    sp_ftbl_create(sp, &ud.ft, 2048);
    sp_ftbl_create(sp, &ud.wt1, 2048);

    sp_gen_line(sp, ud.wt1, "0 1 2048 -1");
    sp_gen_sine(sp, ud.ft);

    sp_ftbl *ft_array[] = {ud.wt1, ud.ft};
    sp_oscmorph_init(sp, ud.oscmorph, ft_array, 2, 0);
    sp_osc_init(sp, ud.osc, ud.ft, 0);
    ud.osc->freq = 1;
    ud.osc->amp = 1;


    for(n = 0; n < tst->size; n++) {
        osc = 0, oscmorph = 0;
        sp_osc_compute(sp, ud.osc, NULL, &osc);
        osc = (1 + osc) * 0.5;
        ud.oscmorph->wtpos = osc;
        sp_oscmorph_compute(sp, ud.oscmorph, NULL, &oscmorph);
        sp_test_add_sample(tst, oscmorph);
    }

    fail = sp_test_verify(tst, hash);

    sp_oscmorph_destroy(&ud.oscmorph);
    sp_ftbl_destroy(&ud.ft);
    sp_ftbl_destroy(&ud.wt1);
    sp_osc_destroy(&ud.osc);

    if(fail) return SP_NOT_OK;
    else return SP_OK;
}
