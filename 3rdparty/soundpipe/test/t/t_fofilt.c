#include "soundpipe.h"
#include "md5.h"
#include "tap.h"
#include "test.h"

typedef struct {
    sp_noise *ns;
    sp_fofilt *ff;
} UserData;

int t_fofilt(sp_test *tst, sp_data *sp, const char *hash) 
{
    sp_srand(sp, 0); 
    uint32_t n;
    int fail = 0;
    SPFLOAT in = 0;

    UserData ud;
    sp_noise_create(&ud.ns);
    sp_fofilt_create(&ud.ff);
    sp_noise_init(sp, ud.ns);
    sp_fofilt_init(sp, ud.ff);
    ud.ff->freq = 500;

    for(n = 0; n < tst->size; n++) {
        in = 0;
        sp_noise_compute(sp, ud.ns, NULL, &in);
        sp_fofilt_compute(sp, ud.ff, &in, &sp->out[0]); 
        sp_test_add_sample(tst, sp->out[0]);
    }

    fail = sp_test_verify(tst, hash);

    sp_noise_destroy(&ud.ns);
    sp_fofilt_destroy(&ud.ff);
 
    if(fail) return SP_NOT_OK;
    else return SP_OK;
}
