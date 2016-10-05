#include "soundpipe.h"
#include "md5.h"
#include "tap.h"
#include "test.h"

typedef struct {
    sp_diskin *diskin;
    sp_conv *conv;
    sp_ftbl *ft; 
} UserData;

int t_conv(sp_test *tst, sp_data *sp, const char *hash) 
{
    uint32_t n;
    int fail = 0;
    SPFLOAT conv = 0, diskin = 0; 

    sp_srand(sp, 1234567);
    UserData ud;

    sp_diskin_create(&ud.diskin);
    sp_conv_create(&ud.conv);
    sp_ftbl_loadfile(sp, &ud.ft, SAMPDIR "imp.wav");

    sp_diskin_init(sp, ud.diskin, SAMPDIR "oneart.wav");
    sp_conv_init(sp, ud.conv, ud.ft, 8192);

    for(n = 0; n < tst->size; n++) {
        conv = 0; diskin = 0;
        sp_diskin_compute(sp, ud.diskin, NULL, &diskin);
        sp_conv_compute(sp, ud.conv, &diskin, &conv);
        sp_test_add_sample(tst, 0);
    }

    fail = sp_test_verify(tst, hash);

    sp_conv_destroy(&ud.conv);
    sp_ftbl_destroy(&ud.ft);
    sp_diskin_destroy(&ud.diskin);

    if(fail) return SP_NOT_OK;
    else return SP_OK;
}
