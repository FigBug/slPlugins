#include "soundpipe.h"
#include "md5.h"
#include "tap.h"
#include "test.h"

typedef struct {
    sp_pareq *pareq;
    sp_noise *noise;
    sp_ftbl *ft; 
} UserData;

int t_pareq(sp_test *tst, sp_data *sp, const char *hash) 
{
    sp_srand(sp, 0); 
    uint32_t n;
    int fail = 0;

    UserData ud;

    sp_pareq_create(&ud.pareq);
    sp_noise_create(&ud.noise);

    sp_pareq_init(sp, ud.pareq);
    ud.pareq->fc = 500;
    sp_noise_init(sp, ud.noise);
    ud.noise->amp = 0.4;

    for(n = 0; n < tst->size; n++) {
        SPFLOAT noise = 0, pareq = 0;
        sp_noise_compute(sp, ud.noise, NULL, &noise);
        sp_pareq_compute(sp, ud.pareq, &noise, &pareq);
        sp->out[0] = pareq;
        sp_test_add_sample(tst, sp->out[0]);
    }

    fail = sp_test_verify(tst, hash);

    sp_pareq_destroy(&ud.pareq);
    sp_noise_destroy(&ud.noise);
 
    if(fail) return SP_NOT_OK;
    else return SP_OK;
}
