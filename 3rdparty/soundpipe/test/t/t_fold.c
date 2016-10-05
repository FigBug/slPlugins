#include "soundpipe.h"
#include "md5.h"
#include "tap.h"
#include "test.h"

typedef struct {
    sp_fold *fold;
    sp_osc *osc;
    sp_ftbl *ft; 
} UserData;

int t_fold(sp_test *tst, sp_data *sp, const char *hash) 
{
    sp_srand(sp, 0);
    uint32_t n;
    int fail = 0;
    UserData ud;

    sp_fold_create(&ud.fold);
    sp_osc_create(&ud.osc);
    sp_ftbl_create(sp, &ud.ft, 2048);

    sp_fold_init(sp, ud.fold);
    sp_gen_sine(sp, ud.ft);
    sp_osc_init(sp, ud.osc, ud.ft, 0);

    sp->len = 44100 * 5;
    for(n = 0; n < tst->size; n++) {
        SPFLOAT osc = 0, fold = 0;
        sp_osc_compute(sp, ud.osc, NULL, &osc);
        sp_fold_compute(sp, ud.fold, &osc, &fold);
        sp->out[0] = fold;
        sp_test_add_sample(tst, sp->out[0]);
    }

    fail = sp_test_verify(tst, hash);

    sp_fold_destroy(&ud.fold);
    sp_ftbl_destroy(&ud.ft);
    sp_osc_destroy(&ud.osc);

    if(fail) return SP_NOT_OK;
    else return SP_OK;
}
