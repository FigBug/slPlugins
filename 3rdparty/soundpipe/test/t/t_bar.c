#include "soundpipe.h"
#include "md5.h"
#include "tap.h"
#include "test.h"

typedef struct {
    sp_bar *bar;
    sp_metro *met;
} UserData;

int t_bar(sp_test *tst, sp_data *sp, const char *hash) 
{
    uint32_t n;
    int fail = 0;
    SPFLOAT bar = 0, met = 0;
    sp_srand(sp, 1234567);
    UserData ud;

    sp_bar_create(&ud.bar);
    sp_metro_create(&ud.met);

    sp_bar_init(sp, ud.bar, 3, 0.0001);
    ud.bar->T30 = 1;

    sp_metro_init(sp, ud.met);
    ud.met->freq = 1;

    for(n = 0; n < tst->size; n++) {
        bar = 0; 
        met = 0;
        sp_metro_compute(sp, ud.met, NULL, &met);
        sp_bar_compute(sp, ud.bar, &met, &bar);
        sp_test_add_sample(tst, bar);
    }

    fail = sp_test_verify(tst, hash);

    sp_bar_destroy(&ud.bar);
    sp_metro_destroy(&ud.met);

    if(fail) return SP_NOT_OK;
    else return SP_OK;
}
