#include "soundpipe.h"
#include "md5.h"
#include "tap.h"
#include "test.h"

typedef struct {
    sp_randh *randh;
    sp_osc *osc;
    sp_ftbl *ft; 
} UserData;

int t_randh(sp_test *tst, sp_data *sp, const char *hash) 
{
    uint32_t n;
    int fail = 0;
    SPFLOAT freq;
    
    UserData ud;
    sp_srand(sp, 12345);
    sp_ftbl_create(sp, &ud.ft, 1024);
    sp_osc_create(&ud.osc);
    sp_randh_create(&ud.randh);

    sp_gen_sine(sp, ud.ft);
    sp_osc_init(sp, ud.osc, ud.ft, 0);
    sp_randh_init(sp, ud.randh);
    
    ud.randh->min = 300;  
    ud.randh->max = 3000;  

    for(n = 0; n < tst->size; n++) {
        sp_randh_compute(sp, ud.randh, NULL, &freq);
        ud.osc->freq = freq;
        sp_osc_compute(sp, ud.osc, NULL, &sp->out[0]);
        sp_test_add_sample(tst, sp->out[0]);
    }

    fail = sp_test_verify(tst, hash);

    sp_ftbl_destroy(&ud.ft);
    sp_osc_destroy(&ud.osc);
    sp_randh_destroy(&ud.randh);

    if(fail) return SP_NOT_OK;
    else return SP_OK;
}
