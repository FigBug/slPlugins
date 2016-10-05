#include "soundpipe.h"
#include "md5.h"
#include "tap.h"
#include "test.h"

typedef struct {
    sp_waveset *waveset;
    sp_diskin *diskin;
} UserData;

int t_waveset(sp_test *tst, sp_data *sp, const char *hash) 
{
    uint32_t n;
    int fail = 0;
    SPFLOAT wav = 0, waveset = 0;

    UserData ud;
    sp_srand(sp, 1234567);

    sp_waveset_create(&ud.waveset);
    sp_diskin_create(&ud.diskin);

    sp_waveset_init(sp, ud.waveset, 1.0);
    ud.waveset->rep = 3.0;
    sp_diskin_init(sp, ud.diskin, SAMPDIR "oneart.wav");

    for(n = 0; n < tst->size; n++) {
        wav = 0; waveset = 0;
        sp_diskin_compute(sp, ud.diskin, NULL, &wav);
        sp_waveset_compute(sp, ud.waveset, &wav, &waveset);
        sp_test_add_sample(tst, waveset);
    }

    fail = sp_test_verify(tst, hash);

    if(fail) return SP_NOT_OK;
    else return SP_OK;
}
