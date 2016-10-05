#include "soundpipe.h"
#include "md5.h"
#include "tap.h"
#include "test.h"

typedef struct {
    sp_autowah *autowah;
    sp_diskin *disk;
} UserData;

int t_autowah(sp_test *tst, sp_data *sp, const char *hash) 
{
    uint32_t n;
    int fail = 0;
    SPFLOAT disk = 0, autowah = 0;

    UserData ud;

    sp_autowah_create(&ud.autowah);
    sp_diskin_create(&ud.disk);

    sp_diskin_init(sp, ud.disk, SAMPDIR "oneart.wav");
    sp_autowah_init(sp, ud.autowah);
    *ud.autowah->wah = 1.0;
    
    for(n = 0; n < tst->size; n++) {
        disk = 0; 
        autowah = 0;
        sp_diskin_compute(sp, ud.disk, NULL, &disk);
        sp_autowah_compute(sp, ud.autowah, &disk, &autowah);
        sp_test_add_sample(tst, autowah);
    }

    fail = sp_test_verify(tst, hash);

    sp_autowah_destroy(&ud.autowah);
    sp_diskin_destroy(&ud.disk);

    if(fail) return SP_NOT_OK;
    else return SP_OK;
}
