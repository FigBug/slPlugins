#include "soundpipe.h"
#include "md5.h"
#include "tap.h"
#include "test.h"

typedef struct {
    sp_fog *fog;
    sp_ftbl *wav;
    sp_ftbl *win;
    sp_phasor *phs;
} UserData;

int t_fog(sp_test *tst, sp_data *sp, const char *hash) 
{
    uint32_t n;
    int fail = 0;
    SPFLOAT phs = 0, fog = 0;

    sp_srand(sp, 1234567);
    UserData ud;

    sp_ftbl_loadfile(sp, &ud.wav, SAMPDIR "oneart.wav");
    sp_ftbl_create(sp, &ud.win, 1024);
    sp_fog_create(&ud.fog);
    sp_phasor_create(&ud.phs);

    sp_gen_composite(sp, ud.win, "0.5 0.5 270 0.5");

    sp_fog_init(sp, ud.fog, ud.wav, ud.win, 100, 0);
    ud.fog->trans = 0.9;

    sp_phasor_init(sp, ud.phs, 0);
    ud.phs->freq = 0.3 / ((SPFLOAT)ud.wav->size / sp->sr); 

    for(n = 0; n < tst->size; n++) {
        phs = 0; 
        fog = 0;
        sp_phasor_compute(sp, ud.phs, NULL, &phs);
        ud.fog->spd = phs;
        sp_fog_compute(sp, ud.fog, NULL, &fog);
        sp_test_add_sample(tst, fog);
    }

    fail = sp_test_verify(tst, hash);

    sp_fog_destroy(&ud.fog);
    sp_ftbl_destroy(&ud.wav);
    sp_ftbl_destroy(&ud.win);
    sp_phasor_destroy(&ud.phs);


    if(fail) return SP_NOT_OK;
    else return SP_OK;
}
