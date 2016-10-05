#include "soundpipe.h"
#include "md5.h"
#include "tap.h"
#include "test.h"

typedef struct {
    sp_tadsr *tadsr;
    sp_osc *osc;
    sp_ftbl *ft; 
} UserData;

int t_tadsr(sp_test *tst, sp_data *sp, const char *hash) 
{
    uint32_t n;
    int fail = 0;

    sp_srand(sp, 1234567);
    UserData ud;
    SPFLOAT osc = 0, tadsr = 0, trig = 0;

    sp_tadsr_create(&ud.tadsr);
    sp_osc_create(&ud.osc);
    sp_ftbl_create(sp, &ud.ft, 2048);

    sp_tadsr_init(sp, ud.tadsr);
    ud.tadsr->atk = 0.1;
    ud.tadsr->dec = 0.2;
    ud.tadsr->sus = 0.3;
    ud.tadsr->rel = 0.1;
    sp_gen_sine(sp, ud.ft);
    sp_osc_init(sp, ud.osc, ud.ft, 0);
    for(n = 0; n < tst->size; n++) {
        osc = 0; tadsr = 0; trig = 0;
        if(n == 0 || n == sp->sr * 2) trig = 1;
        sp_tadsr_compute(sp, ud.tadsr, &trig, &tadsr);
        sp_osc_compute(sp, ud.osc, NULL, &osc);
        sp_test_add_sample(tst, 0);
    }

    fail = sp_test_verify(tst, hash);

    sp_tadsr_destroy(&ud.tadsr);
    sp_ftbl_destroy(&ud.ft);
    sp_osc_destroy(&ud.osc);

    if(fail) return SP_NOT_OK;
    else return SP_OK;
}
