#include "soundpipe.h"
#include "md5.h"
#include "tap.h"
#include "test.h"

int t_foo(sp_test *tst, sp_data *sp, const char *hash) 
{
    uint32_t n;
    int fail = 0;

    /* allocate / initialize modules here */

    for(n = 0; n < tst->size; n++) {
        /* compute samples and add to test buffer */
        sp_test_add_sample(tst, 0);
    }

    fail = sp_test_verify(tst, hash);

    /* destroy functions here */

    if(fail) return SP_NOT_OK;
    /* fail by default */
    else return SP_NOT_OK;
}
