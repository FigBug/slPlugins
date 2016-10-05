#include "soundpipe.h"
#include "md5.h"
#include "tap.h"
#include "test.h"

typedef struct {
    sp_gbuzz *buzz;
    sp_ftbl *ft; 
    int counter;
} UserData;

int t_gbuzz(sp_test *tst, sp_data *sp, const char *hash) 
{
    uint32_t n;
    int fail = 0;

    UserData ud;
    ud.counter = 0;
    sp_ftbl_create(sp, &ud.ft, 2048);
    sp_gbuzz_create(&ud.buzz);
    sp_gen_sine(sp, ud.ft);
    sp_gbuzz_init(sp, ud.buzz, ud.ft, 0);
 
    for(n = 0; n < tst->size; n++) {
        sp_gbuzz_compute(sp, ud.buzz, NULL, &sp->out[0]);
        sp_test_add_sample(tst, sp->out[0]);
    }

    fail = sp_test_verify(tst, hash);

    sp_ftbl_destroy(&ud.ft);
    sp_gbuzz_destroy(&ud.buzz);
     
    if(fail) return SP_NOT_OK;
    else return SP_OK;
}
