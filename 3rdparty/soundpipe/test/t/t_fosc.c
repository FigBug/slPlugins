#include "soundpipe.h"
#include "md5.h"
#include "tap.h"
#include "test.h"

typedef struct {
    sp_fosc *osc;
    sp_ftbl *ft; 
    int counter;
} UserData;

int t_fosc(sp_test *tst, sp_data *sp, const char *hash) 
{
    sp_srand(sp, 0);
    uint32_t n;
    int fail = 0;
    UserData ud;
    ud.counter = 0;
    sp_ftbl_create(sp, &ud.ft, 2048);
    sp_fosc_create(&ud.osc);

    sp_gen_sine(sp, ud.ft);
    sp_fosc_init(sp, ud.osc, ud.ft);
    ud.osc->freq = 500;

    for(n = 0; n < tst->size; n++) {
        if(ud.counter == 0){
            ud.osc->freq = 500 + sp_rand(sp) % 2000;
        }

        sp_fosc_compute(sp, ud.osc, NULL, &sp->out[0]);
        ud.counter = (ud.counter + 1) % 4410;

        sp_test_add_sample(tst, sp->out[0]);
    }


    fail = sp_test_verify(tst, hash);

    sp_ftbl_destroy(&ud.ft);
    sp_fosc_destroy(&ud.osc);

    if(fail) return SP_NOT_OK;
    else return SP_OK;
}
