#include "soundpipe.h"
#include "md5.h"
#include "tap.h"
#include "test.h"

typedef struct user_data {
    sp_ftbl *ft, *amps;
    sp_osc *osc;
    SPFLOAT fc;
} UserData;

int t_padsynth(sp_test *tst, sp_data *sp, const char *hash) 
{
    uint32_t n;
    int fail = 0;
    SPFLOAT out = 0;

    UserData ud;
    int i;
    sp_srand(sp, 12345);
    sp_ftbl_create(sp, &ud.amps, 64);
    sp_ftbl_create(sp, &ud.ft, 262144);
    sp_osc_create(&ud.osc);

    ud.amps->tbl[0] = 0.0;

    for(i = 1; i < ud.amps->size; i++){
        ud.amps->tbl[i] = 1.0 / i;
        if((i % 2) == 0) ud.amps->tbl[i] *= 2.0; 
    }

    
    /* Discovered empirically. multiply frequency by this constant. */
    ud.fc = 1 / (6.0 * 440); 
    sp_gen_padsynth(sp, ud.ft, ud.amps, sp_midi2cps(60), 40.0);

    sp_osc_init(sp, ud.osc, ud.ft, 0);
    ud.osc->freq = sp_midi2cps(70) * ud.fc;
    ud.osc->amp = 1.0;

    for(n = 0; n < tst->size; n++) {
        sp_osc_compute(sp, ud.osc, NULL, &out);
        sp_test_add_sample(tst, out);
    }

    fail = sp_test_verify(tst, hash);

    sp_osc_destroy(&ud.osc);
    sp_ftbl_destroy(&ud.amps);
    sp_ftbl_destroy(&ud.ft);

    if(fail) return SP_NOT_OK;
    else return SP_OK;
}
