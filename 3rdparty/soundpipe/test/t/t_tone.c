#include "soundpipe.h"
#include "md5.h"
#include "tap.h"
#include "test.h"

typedef struct {
    sp_noise *ns;
    sp_tone *tn;
} UserData;

int t_tone(sp_test *tst, sp_data *sp, const char *hash) 
{
    uint32_t n;
    int fail = 0;

    UserData ud;
    sp_noise_create(&ud.ns);
    sp_tone_create(&ud.tn);
    sp_noise_init(sp, ud.ns);
    sp_tone_init(sp, ud.tn);

    SPFLOAT in;

    for(n = 0; n < tst->size; n++) {
        sp_noise_compute(sp, ud.ns, NULL, &in);
        sp_tone_compute(sp, ud.tn, &in, &sp->out[0]); 
        sp_test_add_sample(tst, sp->out[0]);
    }
    
    fail = sp_test_verify(tst, hash);

    sp_tone_destroy(&ud.tn);
    sp_noise_destroy(&ud.ns);

    if(fail) return SP_NOT_OK;
    else return SP_OK;
}
