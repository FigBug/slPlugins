#include "soundpipe.h"
#include "md5.h"
#include "tap.h"
#include "test.h"

typedef struct {
    sp_diskin *diskin;
    sp_osc *osc;
    sp_ftbl *ft; 
} UserData;

int t_diskin(sp_test *tst, sp_data *sp, const char *hash) 
{
    uint32_t n;
    int fail = 0;
    SPFLOAT diskin = 0;

    sp_srand(sp, 1234567);
    UserData ud;

    sp_diskin_create(&ud.diskin);
    sp_diskin_init(sp, ud.diskin, SAMPDIR "oneart.wav");

    for(n = 0; n < tst->size; n++) {
        diskin = 0;
        sp_diskin_compute(sp, ud.diskin, NULL, &diskin);
        sp_test_add_sample(tst, diskin);
    }

    fail = sp_test_verify(tst, hash);

    sp_diskin_destroy(&ud.diskin);

    if(fail) return SP_NOT_OK;
    else return SP_OK;
}
