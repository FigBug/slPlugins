#include "soundpipe.h"
#include "md5.h"
#include "tap.h"
#include "test.h"

int t_gen_xline(sp_test *tst, sp_data *sp, const char *hash) 
{
    uint32_t n;
    int fail = 0;
    SPFLOAT out = 0;

    sp_ftbl *ft;
    sp_ftbl_create(sp, &ft, 4096);
    sp_gen_xline(sp, ft, "0 0.0001 4096 1.0");

    for(n = 0; n < tst->size; n++) {
        if(n < ft->size) {
            out = ft->tbl[n];
        } else {
            out = 0;
        }
        sp_test_add_sample(tst, out);
    }

    fail = sp_test_verify(tst, hash);

    sp_ftbl_destroy(&ft);

    if(fail) return SP_NOT_OK;
    else return SP_OK;
}
