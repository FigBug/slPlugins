#include "soundpipe.h"
#include "md5.h"
#include "tap.h"
#include "test.h"

typedef struct {
    sp_fosc *osc;
    sp_ftbl *ft; 
    sp_jcrev *dsp;
    int counter;
} UserData;

int t_jcrev(sp_test *tst, sp_data *sp, const char *hash) 
{
    uint32_t n;
    int fail = 0;
    UserData ud;
    SPFLOAT in = 0, out = 0;

    sp_srand(sp, 123456);
    sp_jcrev_create(&ud.dsp);
    sp_ftbl_create(sp, &ud.ft, 2048);
    sp_fosc_create(&ud.osc);

    sp_jcrev_init(sp, ud.dsp);
    ud.counter = 0;
    sp_gen_sine(sp, ud.ft);
    sp_fosc_init(sp, ud.osc, ud.ft);
    ud.osc->freq = 500;

    for(n = 0; n < tst->size; n++) {
        in = 0, out = 0;

        if(ud.counter == 0){
            ud.osc->freq = 500 + sp_rand(sp) % 2000;
        }

        sp_fosc_compute(sp, ud.osc, NULL, &in);
        sp_jcrev_compute(sp, ud.dsp, &in, &out);

        sp->out[0] = out;
        ud.counter = (ud.counter + 1) % 4410;
        sp_test_add_sample(tst, sp->out[0]);
    }

    fail = sp_test_verify(tst, hash);
    
    sp_jcrev_destroy(&ud.dsp);
    sp_ftbl_destroy(&ud.ft);
    sp_fosc_destroy(&ud.osc);

    if(fail) return SP_NOT_OK;
    else return SP_OK;
}
