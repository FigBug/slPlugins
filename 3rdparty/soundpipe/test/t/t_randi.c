#include "soundpipe.h"
#include "md5.h"
#include "tap.h"
#include "test.h"

typedef struct {
    sp_osc *osc;
    sp_ftbl *ft; 
    sp_randi *rnd;
} UserData;

int t_randi(sp_test *tst, sp_data *sp, const char *hash) 
{
    uint32_t n;
    int fail = 0;
    SPFLOAT freq;
    
    UserData ud;
    sp_srand(sp, 12345);
    sp_ftbl_create(sp, &ud.ft, 1024);
    sp_osc_create(&ud.osc);
    sp_randi_create(&ud.rnd);

    sp_gen_sine(sp, ud.ft);
    sp_osc_init(sp, ud.osc, ud.ft, 0);
    sp_randi_init(sp, ud.rnd);
    
    ud.rnd->min = 300;  
    ud.rnd->max = 3000;  

    for(n = 0; n < tst->size; n++) {
        sp_randi_compute(sp, ud.rnd, NULL, &freq);
        ud.osc->freq = freq;
        sp_osc_compute(sp, ud.osc, NULL, &sp->out[0]);
        sp_test_add_sample(tst, sp->out[0]);
    }

    fail = sp_test_verify(tst, hash);

    sp_ftbl_destroy(&ud.ft);
    sp_osc_destroy(&ud.osc);
    sp_randi_destroy(&ud.rnd);

    if(fail) return SP_NOT_OK;
    else return SP_OK;
}
