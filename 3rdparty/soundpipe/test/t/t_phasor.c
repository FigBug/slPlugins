#include "soundpipe.h"
#include "md5.h"
#include "tap.h"
#include "test.h"

typedef struct {
    sp_phasor *phs;
} UserData;

int t_phasor(sp_test *tst, sp_data *sp, const char *hash) 
{
    uint32_t n;
    int fail = 0;

    UserData ud;
    sp_phasor_create(&ud.phs);
    sp_phasor_init(sp, ud.phs, 0);

    for(n = 0; n < tst->size; n++) {
        sp_phasor_compute(sp, ud.phs, NULL, &sp->out[0]);
        sp_test_add_sample(tst, sp->out[0]);
    }

    fail = sp_test_verify(tst, hash);

    sp_phasor_destroy(&ud.phs);
     
    if(fail) return SP_NOT_OK;
    else return SP_OK;
}
