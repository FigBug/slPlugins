#include "soundpipe.h"
#include "md5.h"
#include "tap.h"
#include "test.h"

typedef struct {
    sp_peaklim *peaklim;
    sp_diskin *diskin;
} UserData;

int t_peaklim(sp_test *tst, sp_data *sp, const char *hash) 
{
    uint32_t n;
    int fail = 0;
    SPFLOAT diskin = 0, peaklim = 0;

    UserData ud;
    sp_srand(sp, 1234567);

    sp_peaklim_create(&ud.peaklim);
    sp_diskin_create(&ud.diskin);

    sp_peaklim_init(sp, ud.peaklim);
    ud.peaklim->atk = 0.1;
    ud.peaklim->rel = 0.1;
    ud.peaklim->thresh = -30;
    sp_diskin_init(sp, ud.diskin, SAMPDIR "oneart.wav");

    for(n = 0; n < tst->size; n++) {
        diskin = 0; peaklim = 0;
        sp_diskin_compute(sp, ud.diskin, NULL, &diskin);
        sp_peaklim_compute(sp, ud.peaklim, &diskin, &peaklim);
        sp_test_add_sample(tst, peaklim);
    }

    fail = sp_test_verify(tst, hash);

    sp_peaklim_destroy(&ud.peaklim);
    sp_diskin_destroy(&ud.diskin);

    if(fail) return SP_NOT_OK;
    /* fail by default */
    else return SP_OK;
}
