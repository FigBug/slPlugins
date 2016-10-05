#include "soundpipe.h"
#include "md5.h"
#include "tap.h"
#include "test.h"

typedef struct {
    sp_bitcrush *bitcrush;
    sp_noise *nz;
} UserData;

int t_bitcrush(sp_test *tst, sp_data *sp, const char *hash)
{
    sp_srand(sp, 0);
    uint32_t n;
    int fail = 0;
    SPFLOAT noise = 0, bitcrush = 0;
    UserData ud;

    sp_bitcrush_create(&ud.bitcrush);
    sp_noise_create(&ud.nz);

    sp_noise_init(sp, ud.nz);
    sp_bitcrush_init(sp, ud.bitcrush);
    ud.bitcrush->bitdepth = 8;
    ud.bitcrush->srate = 10000;

    for(n = 0; n < tst->size; n++) {
        noise = 0, bitcrush = 0;
        sp_noise_compute(sp, ud.nz, NULL, &noise);
        sp_bitcrush_compute(sp, ud.bitcrush, &noise, &bitcrush);
        sp_test_add_sample(tst, bitcrush);
    }

    fail = sp_test_verify(tst, hash);

    sp_bitcrush_destroy(&ud.bitcrush);
    sp_noise_destroy(&ud.nz);

    if(fail) return SP_NOT_OK;
    else return SP_OK;
}
