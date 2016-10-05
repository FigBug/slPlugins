#include <math.h>
#include "soundpipe.h"
#include "md5.h"
#include "tap.h"
#include "test.h"

typedef struct {
    sp_noise *ns;
    sp_eqfil *eqfil;
    int counter;
} UserData;

int t_eqfil(sp_test *tst, sp_data *sp, const char *hash) 
{
    sp_srand(sp, 0); 
    uint32_t n;
    int fail = 0;
    UserData ud;
    SPFLOAT in = 0;
    SPFLOAT out = 0;
    
    ud.counter = 0;
    sp_noise_create(&ud.ns);
    sp_eqfil_create(&ud.eqfil);
    sp_noise_init(sp, ud.ns);
    sp_eqfil_init(sp, ud.eqfil);

    for(n = 0; n < tst->size; n++) {
        in = 0;
        out = 0;
        if(ud.counter == 0) {
            ud.eqfil->freq = 500 + sp_rand(sp) % 4000;
            ud.eqfil->bw = fabs(ud.eqfil->freq * 0.5);
        }
        sp_noise_compute(sp, ud.ns, NULL, &in);
        sp_eqfil_compute(sp, ud.eqfil, &in, &out); 
        ud.counter = (ud.counter + 1) % 5000;
        sp_test_add_sample(tst, out);
    }

    fail = sp_test_verify(tst, hash);
     
    sp_noise_destroy(&ud.ns);
    sp_eqfil_destroy(&ud.eqfil);

    if(fail) return SP_NOT_OK;
    else return SP_OK;
}
