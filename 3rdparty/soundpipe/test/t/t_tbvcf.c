#include "soundpipe.h"
#include "md5.h"
#include "tap.h"
#include "test.h"


typedef struct {
    sp_noise *ns;
    sp_tbvcf *tn;
} UserData;

int t_tbvcf(sp_test *tst, sp_data *sp, const char *hash) 
{
    uint32_t n;
    int fail = 0;

    sp_srand(sp, 0); 
    UserData ud;
    sp_noise_create(&ud.ns);
    sp_tbvcf_create(&ud.tn);
    sp_noise_init(sp, ud.ns);
    sp_tbvcf_init(sp, ud.tn);
    ud.tn->dist = 1.0;
    SPFLOAT in = 0;

    for(n = 0; n < tst->size; n++) {
        in = 0;

        sp_noise_compute(sp, ud.ns, NULL, &in);
        sp_tbvcf_compute(sp, ud.tn, &in, &sp->out[0]); 
        sp_test_add_sample(tst, sp->out[0]);
    }

    fail = sp_test_verify(tst, hash);
    
    sp_noise_destroy(&ud.ns);
    sp_tbvcf_destroy(&ud.tn);

    if(fail) return SP_NOT_OK;
    else return SP_OK;
}
