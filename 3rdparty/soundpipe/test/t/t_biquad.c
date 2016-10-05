#include "soundpipe.h"
#include "md5.h"
#include "tap.h"
#include "test.h"

typedef struct udata {
    sp_noise *ns;
    sp_biquad *tn;
} UserData;

int t_biquad(sp_test *tst, sp_data *sp, const char *hash) 
{
    sp_srand(sp, 0); 
    uint32_t n;
    int fail = 0;
    SPFLOAT in = 0;
    SPFLOAT out = 0;

    UserData ud;

    sp_noise_create(&ud.ns);
    sp_biquad_create(&ud.tn);

    sp_noise_init(sp, ud.ns);
    sp_biquad_init(sp, ud.tn);

    for(n = 0; n < tst->size; n++) {
        in = 0;
        out = 0;
        sp_noise_compute(sp, ud.ns, NULL, &in);
        sp_biquad_compute(sp, ud.tn, &in, &out); 
        sp_test_add_sample(tst, out);
    }

    fail = sp_test_verify(tst, hash);
    
    sp_noise_destroy(&ud.ns);
    sp_biquad_destroy(&ud.tn);
     
    if(fail) return SP_NOT_OK;
    else return SP_OK;
}
