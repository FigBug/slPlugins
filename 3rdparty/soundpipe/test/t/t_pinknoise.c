#include "soundpipe.h"
#include "md5.h"
#include "tap.h"
#include "test.h"

typedef struct {
    sp_pinknoise *ns;
} UserData;

int t_pinknoise(sp_test *tst, sp_data *sp, const char *hash) 
{
    uint32_t n;
    int fail = 0;
    SPFLOAT out = 0;

    UserData ud;
    sp_srand(sp, 12345);
    sp_pinknoise_create(&ud.ns);
    sp_pinknoise_init(sp, ud.ns);

    for(n = 0; n < tst->size; n++) {
        out = 0;
        sp_pinknoise_compute(sp, ud.ns, NULL, &out);
        sp_test_add_sample(tst, out);
    }

    fail = sp_test_verify(tst, hash);

    sp_pinknoise_destroy(&ud.ns);

    if(fail) return SP_NOT_OK;
    else return SP_OK;
}
