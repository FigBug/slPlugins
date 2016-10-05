#include "soundpipe.h"
#include "md5.h"
#include "tap.h"
#include "test.h"

typedef struct {
    sp_dust *dst;
} UserData;

int t_dust(sp_test *tst, sp_data *sp, const char *hash) 
{
    uint32_t n;
    int fail = 0;
    SPFLOAT out = 0;
    UserData ud;
    
    sp_dust_create(&ud.dst);
    sp_dust_init(sp, ud.dst);
    sp->len = 44100 * 5;
    ud.dst->bipolar = 1.0;

    for(n = 0; n < tst->size; n++) {
        out = 0;
        sp_dust_compute(sp, ud.dst, NULL, &out);
        sp_test_add_sample(tst, out);
    }

    fail = sp_test_verify(tst, hash);

    sp_dust_destroy(&ud.dst);
     
    if(fail) return SP_NOT_OK;
    else return SP_OK;
}
