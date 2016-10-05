#include "soundpipe.h"
#include "md5.h"
#include "tap.h"
#include "test.h"

typedef struct {
    sp_compressor *compressor;
    sp_diskin *diskin;
} UserData;

int t_compressor(sp_test *tst, sp_data *sp, const char *hash) 
{
    uint32_t n;
    int fail = 0;
    SPFLOAT diskin = 0, compressor = 0;

    sp_srand(sp, 1234567);
    UserData ud;

    sp_compressor_create(&ud.compressor);
    sp_diskin_create(&ud.diskin);

    sp_compressor_init(sp, ud.compressor);
    *ud.compressor->ratio = 4;
    *ud.compressor->thresh = -30;
    *ud.compressor->atk = 0.2;
    *ud.compressor->rel = 0.2;

    sp_diskin_init(sp, ud.diskin, SAMPDIR "oneart.wav");

    for(n = 0; n < tst->size; n++) {
        diskin = 0; compressor = 0;
        sp_diskin_compute(sp, ud.diskin, NULL, &diskin);
        sp_compressor_compute(sp, ud.compressor, &diskin, &compressor);
        sp_test_add_sample(tst, compressor);
    }

    fail = sp_test_verify(tst, hash);

    sp_compressor_destroy(&ud.compressor);
    sp_diskin_destroy(&ud.diskin);

    if(fail) return SP_NOT_OK;
    else return SP_OK;
}
