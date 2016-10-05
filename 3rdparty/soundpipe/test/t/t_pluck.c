#include "soundpipe.h"
#include "md5.h"
#include "tap.h"
#include "test.h"

typedef struct {
    sp_pluck *pluck;
    sp_metro *met;
} UserData;

int t_pluck(sp_test *tst, sp_data *sp, const char *hash) 
{
    uint32_t n;
    int fail = 0;

    UserData ud;
    sp_srand(sp, 1337); 
    SPFLOAT pluck = 0, met = 0;
    SPFLOAT notes[] = {60, 63, 67, 70, 74};

    sp_pluck_create(&ud.pluck);
    sp_metro_create(&ud.met);

    sp_pluck_init(sp, ud.pluck, 400);
    sp_metro_init(sp, ud.met);
    ud.met->freq = 4;

    for(n = 0; n < tst->size; n++) {
        pluck = 0, met = 0;
        sp_metro_compute(sp, ud.met, NULL, &met);
        if(met) {
            ud.pluck->freq = sp_midi2cps(notes[sp_rand(sp) % 5]);
        }
        sp_pluck_compute(sp, ud.pluck, &met, &pluck);
        sp_test_add_sample(tst, pluck);
    }
    
    fail = sp_test_verify(tst, hash);

    sp_pluck_destroy(&ud.pluck);
    sp_metro_destroy(&ud.met);

    if(fail) return SP_NOT_OK;
    else return SP_OK;
}
