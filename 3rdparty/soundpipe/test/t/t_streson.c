#include "soundpipe.h"
#include "md5.h"
#include "tap.h"
#include "test.h"

typedef struct {
    sp_noise *ns;
    sp_streson *stres;
} UserData;

int t_streson(sp_test *tst, sp_data *sp, const char *hash) 
{
    uint32_t n;
    int fail = 0;
    SPFLOAT noise = 0;

    sp_srand(sp, 123456);
    UserData ud;

    sp_streson_create(&ud.stres);
    sp_noise_create(&ud.ns);
    sp_noise_init(sp, ud.ns);
    sp_streson_init(sp, ud.stres);
    sp->len = 44100 * 5;

    for(n = 0; n < tst->size; n++) {
        noise = 0;
        sp_noise_compute(sp, ud.ns, NULL, &noise);
        sp_streson_compute(sp, ud.stres, &noise, &sp->out[0]);
        sp_test_add_sample(tst, sp->out[0]);
    }

    fail = sp_test_verify(tst, hash);
    
    sp_noise_destroy(&ud.ns);
    sp_streson_destroy(&ud.stres);

    if(fail) return SP_NOT_OK;
    else return SP_OK;
}
