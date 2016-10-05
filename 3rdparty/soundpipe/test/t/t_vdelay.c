#include "soundpipe.h"
#include "md5.h"
#include "tap.h"
#include "test.h"

typedef struct {
    sp_osc *osc, *lfo;
    sp_ftbl *ft, *sine;
    sp_vdelay *del;
} UserData;

int t_vdelay(sp_test *tst, sp_data *sp, const char *hash) 
{
    uint32_t n;
    int fail = 0;
    UserData ud;
    SPFLOAT freq = 0.0, del = 0.0, osc = 0.0;

    sp_vdelay_create(&ud.del);
    sp_ftbl_create(sp, &ud.ft, 2048);
    sp_osc_create(&ud.osc);
    sp_ftbl_create(sp, &ud.sine, 4096);
    sp_osc_create(&ud.lfo);

    sp_vdelay_init(sp, ud.del, 0.5);
    sp_gen_file(sp, ud.ft, "Formant04.wav");
    sp_osc_init(sp, ud.osc, ud.ft, 0);
    ud.osc->freq = sp_midi2cps(50);

    sp_gen_sine(sp, ud.sine);
    sp_osc_init(sp, ud.lfo, ud.sine, 0);
    ud.lfo->freq = 0.2;

    for(n = 0; n < tst->size; n++) {
        freq = 0.0, del = 0.0, osc = 0.0;
        sp_osc_compute(sp, ud.lfo, NULL, &freq);
        freq = 1.0 + (freq * 0.5);
        freq = 0.02 + (freq * 0.02);
        ud.del->del = freq;
        sp_osc_compute(sp, ud.osc, NULL, &osc);
        sp_vdelay_compute(sp, ud.del, &osc, &del);
        sp->out[0] = del * 0.5 + osc * 0.5;
        sp_test_add_sample(tst, sp->out[0]);
    }

    fail = sp_test_verify(tst, hash);
    
    sp_vdelay_destroy(&ud.del);
    sp_ftbl_destroy(&ud.ft);
    sp_ftbl_destroy(&ud.sine);
    sp_osc_destroy(&ud.osc);
    sp_osc_destroy(&ud.lfo);

    if(fail) return SP_NOT_OK;
    else return SP_OK;
}
