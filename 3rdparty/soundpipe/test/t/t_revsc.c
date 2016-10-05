#include "soundpipe.h"
#include "md5.h"
#include "tap.h"
#include "test.h"

typedef struct {
    sp_noise *ns;
    sp_revsc *rev;
    int counter;
} UserData;

int t_revsc(sp_test *tst, sp_data *sp, const char *hash) 
{
    uint32_t n;
    int fail = 0;
    SPFLOAT in = 0;
    SPFLOAT foo = 0;
    sp_srand(sp, 123456);
    UserData ud;
    ud.counter = 0;
    sp_noise_create(&ud.ns);
    sp_revsc_create(&ud.rev);
    sp_noise_init(sp, ud.ns);
    sp_revsc_init(sp, ud.rev);

    sp->len = 44100 * 5;

    for(n = 0; n < tst->size; n++) {
        in = 0;
        foo = 0;
        sp_noise_compute(sp, ud.ns, NULL, &in);
        
        if(ud.counter < 2000) {
            ud.counter = (ud.counter + 1) % 5000;
        }else{
            in = 0;
        }
        sp_revsc_compute(sp, ud.rev, &in, &in, &sp->out[0], &foo); 
        sp_test_add_sample(tst, sp->out[0]);
    }

    fail = sp_test_verify(tst, hash);
    
    sp_noise_destroy(&ud.ns);
    sp_revsc_destroy(&ud.rev);

    if(fail) return SP_NOT_OK;
    else return SP_OK;
}
