#include "soundpipe.h"
#include "md5.h"
#include "tap.h"
#include "test.h"

typedef struct {
    sp_adsr *adsr;
    sp_osc *osc;
    sp_ftbl *ft; 
} UserData;

int t_adsr(sp_test *tst, sp_data *sp, const char *hash) 
{
    sp_srand(sp, 12345);
    uint32_t n;
    int fail = 0;
    UserData ud; 
    sp_adsr_create(&ud.adsr);
    sp_osc_create(&ud.osc);
    sp_ftbl_create(sp, &ud.ft, 8192);
    SPFLOAT osc = 0, adsr = 0, gate = 0;
    

    sp_adsr_init(sp, ud.adsr);
    sp_gen_sine(sp, ud.ft);
    sp_osc_init(sp, ud.osc, ud.ft, 0);
    ud.osc->amp = 0.5;


    /* allocate / initialize modules here */

    for(n = 0; n < tst->size; n++) {
        if(n < tst->size * 0.5) {
            gate = 1;
        } else {
            gate = 0;
        }
        sp_adsr_compute(sp, ud.adsr, &gate, &adsr);
        sp_osc_compute(sp, ud.osc, NULL, &osc);
        sp_test_add_sample(tst, adsr * osc);
    }

    fail = sp_test_verify(tst, hash);

    /* destroy functions here */

    sp_adsr_destroy(&ud.adsr);
    sp_ftbl_destroy(&ud.ft);
    sp_osc_destroy(&ud.osc);

    if(fail) return SP_NOT_OK;
    /* fail by default */
    else return SP_OK;

}
