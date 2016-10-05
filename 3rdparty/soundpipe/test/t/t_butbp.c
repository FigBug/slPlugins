#include "soundpipe.h"
#include "md5.h"
#include "tap.h"
#include "test.h"

typedef struct {
    sp_noise *ns;
    sp_butbp *butbp;
    int counter;
} UserData;

int t_butbp(sp_test *tst, sp_data *sp, const char *hash) 
{
    sp_srand(sp, 0); 
    uint32_t n;
    int fail = 0;
    SPFLOAT in = 0;
    SPFLOAT out = 0;
    UserData ud;
    ud.counter = 0;
    sp_noise_create(&ud.ns);
    sp_butbp_create(&ud.butbp);
    sp_noise_init(sp, ud.ns);
    sp_butbp_init(sp, ud.butbp);

    for(n = 0; n < tst->size; n++) {
        in = 0;
        out = 0;
        if(ud.counter == 0) {
            ud.butbp->freq= 500 + sp_rand(sp) % 4000;
        }
        sp_noise_compute(sp, ud.ns, NULL, &in);
        sp_butbp_compute(sp, ud.butbp, &in, &out); 
        ud.counter = (ud.counter + 1) % 5000;
        sp_test_add_sample(tst, out);
    }

    fail = sp_test_verify(tst, hash);
    
    sp_noise_destroy(&ud.ns);
    sp_butbp_destroy(&ud.butbp);
     
    if(fail) return SP_NOT_OK;
    else return SP_OK;
}
