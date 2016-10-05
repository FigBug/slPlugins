#include "soundpipe.h"
#include "md5.h"
#include "tap.h"
#include "test.h"

typedef struct {
    sp_metro *met;
    sp_nsmp *nsmp;
    sp_ftbl *ft;
} UserData;

int t_nsmp(sp_test *tst, sp_data *sp, const char *hash) 
{
    uint32_t n;
    int fail = 0;
    SPFLOAT met = 0, nsmp = 0;
    sp_srand(sp, 1234567);
    UserData ud;

    sp_ftbl_loadfile(sp, &ud.ft, SAMPDIR "oneart.wav");
    sp_nsmp_create(&ud.nsmp);
    sp_metro_create(&ud.met);

    sp_metro_init(sp, ud.met);
    ud.met->freq = 2;
    sp_nsmp_init(sp, ud.nsmp, ud.ft, 44100, SAMPDIR "oneart.ini");

    for(n = 0; n < tst->size; n++) {
        met = 0; nsmp = 0;
        sp_metro_compute(sp, ud.met, NULL, &met);
        if(met) {
            ud.nsmp->index = sp_rand(sp) % 3;
        }
        sp_nsmp_compute(sp, ud.nsmp, &met, &nsmp);
        sp_test_add_sample(tst, nsmp);
    }

    fail = sp_test_verify(tst, hash);

    sp_nsmp_destroy(&ud.nsmp);
    sp_metro_destroy(&ud.met);

    sp_ftbl_destroy(&ud.ft);

    if(fail) return SP_NOT_OK;
    else return SP_OK;
}
