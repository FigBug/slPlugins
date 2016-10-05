#include "soundpipe.h"
#include "md5.h"
#include "tap.h"
#include "test.h"

typedef struct {
    sp_tseg *tseg;
    sp_osc *osc;
    sp_ftbl *ft; 
} UserData;

int t_tseg(sp_test *tst, sp_data *sp, const char *hash) 
{
    uint32_t n;
    int fail = 0;
    sp_srand(sp, 1234567);
    UserData ud;
    SPFLOAT osc = 0, tseg = 0;
    SPFLOAT trig = (sp->pos == 0);

    sp_tseg_create(&ud.tseg);
    sp_osc_create(&ud.osc);
    sp_ftbl_create(sp, &ud.ft, 2048);

    sp_tseg_init(sp, ud.tseg, 0.0001);
    ud.tseg->end = 1.0;
    ud.tseg->type = 3.0;
    ud.tseg->dur = 4.0;

    sp_gen_sine(sp, ud.ft);
    sp_osc_init(sp, ud.osc, ud.ft, 0);

    for(n = 0; n < tst->size; n++) {
        osc = 0; 
        tseg = 0;
        trig = (sp->pos == 0);
        sp_tseg_compute(sp, ud.tseg, &trig, &tseg);
        ud.osc->freq = 100 + (tseg * 1000);
        sp_osc_compute(sp, ud.osc, NULL, &osc);
        sp_test_add_sample(tst, osc);
    }

    fail = sp_test_verify(tst, hash);

    sp_tseg_destroy(&ud.tseg);
    sp_ftbl_destroy(&ud.ft);
    sp_osc_destroy(&ud.osc);

    if(fail) return SP_NOT_OK;
    else return SP_OK;
}
