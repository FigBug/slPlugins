#include "soundpipe.h"
#include "md5.h"
#include "tap.h"
#include "test.h"

typedef struct {
    sp_fof *fof;
    sp_ftbl *sine;
    sp_ftbl *win;
} UserData;

int t_fof(sp_test *tst, sp_data *sp, const char *hash) 
{
    uint32_t n;
    int fail = 0;
    SPFLOAT osc = 0, fof = 0;

    sp_srand(sp, 1234567);
    UserData ud;

    sp_ftbl_create(sp, &ud.sine, 2048);
    sp_ftbl_create(sp, &ud.win, 1024);
    sp_fof_create(&ud.fof);

    sp_gen_sine(sp, ud.sine);
    sp_gen_composite(sp, ud.win, "0.5 0.5 270 0.5");

    sp_fof_init(sp, ud.fof, ud.sine, ud.win, 100, 0);

    for(n = 0; n < tst->size; n++) {
        osc = 0;
        fof = 0;
        sp_fof_compute(sp, ud.fof, &osc, &fof);
        sp_test_add_sample(tst, fof);
    }

    fail = sp_test_verify(tst, hash);
    
    sp_fof_destroy(&ud.fof);
    sp_ftbl_destroy(&ud.sine);
    sp_ftbl_destroy(&ud.win);

    if(fail) return SP_NOT_OK;
    else return SP_OK;
}
