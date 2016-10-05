#include "soundpipe.h"
#include "md5.h"
#include "tap.h"
#include "test.h"

typedef struct {
    sp_diskin *disk;
    sp_zitarev *rev;
    int counter;
} UserData;

int t_zitarev(sp_test *tst, sp_data *sp, const char *hash) 
{
    uint32_t n;
    int fail = 0;

    SPFLOAT in = 0;
    SPFLOAT out = 0;
    SPFLOAT foo = 0;

    sp_srand(sp,12345);
    UserData ud;
    ud.counter = 0;
    sp_diskin_create(&ud.disk);
    sp_zitarev_create(&ud.rev);

    sp_diskin_init(sp, ud.disk, SAMPDIR "oneart.wav");
    sp_zitarev_init(sp, ud.rev);
    *ud.rev->level = 0;
    *ud.rev->in_delay = 20;

    for(n = 0; n < tst->size; n++) {
        in = 0;
        out = 0;
        foo = 0;

        sp_diskin_compute(sp, ud.disk, NULL, &in);
        sp_zitarev_compute(sp, ud.rev, &in, &in, &out, &foo);
        sp_test_add_sample(tst, out);
    }

    fail = sp_test_verify(tst, hash);

    sp_diskin_destroy(&ud.disk);
    sp_zitarev_destroy(&ud.rev);

    if(fail) return SP_NOT_OK;
    else return SP_OK;
}
