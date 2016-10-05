#include "soundpipe.h"
#include "md5.h"
#include "tap.h"
#include "test.h"

typedef struct {
    sp_random *random;
} UserData;

int t_random(sp_test *tst, sp_data *sp, const char *hash) 
{
    uint32_t n;
    int fail = 0;
    SPFLOAT random = 0;
    
    sp_srand(sp,1234567);
    UserData ud;
    
    sp_random_create(&ud.random);

    sp_random_init(sp, ud.random);
    ud.random->min = -0.2;
    ud.random->max = 0.2;

    for(n = 0; n < tst->size; n++) {
        /* compute samples and add to test buffer */
        random = 0;
        sp_random_compute(sp, ud.random, NULL, &random);
        sp->out[0] = random;
        sp_test_add_sample(tst, sp->out[0]);
    }

    fail = sp_test_verify(tst, hash);

    /* destroy functions here */
    sp_random_destroy(&ud.random);

    if(fail) return SP_NOT_OK;
    else return SP_OK;
}
