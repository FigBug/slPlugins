#include "soundpipe.h"
#include "md5.h"
#include "tap.h"
#include "test.h"

typedef struct {
    sp_posc3 *posc3;
    sp_ftbl *ft; 
    int counter;
} UserData;

int t_posc3(sp_test *tst, sp_data *sp, const char *hash) 
{
    uint32_t n;
    int fail = 0;
    
    sp_srand(sp, 123456);
    UserData ud;
    ud.counter = 0;
    sp_ftbl_create(sp, &ud.ft, 1024);
    sp_posc3_create(&ud.posc3);
    
    sp_gen_sine(sp, ud.ft);
    sp_posc3_init(sp, ud.posc3, ud.ft);
    ud.posc3->freq = 500;

    for(n = 0; n < tst->size; n++) {
        if(ud.counter == 0){
            ud.posc3->freq = 500 + sp_rand(sp) % 2000;
        }
        sp_posc3_compute(sp, ud.posc3, NULL, &sp->out[0]);
        ud.counter = (ud.counter + 1) % 4410;
        sp_test_add_sample(tst, sp->out[0]);
    }

    fail = sp_test_verify(tst, hash);

    sp_ftbl_destroy(&ud.ft);
    sp_posc3_destroy(&ud.posc3);

    if(fail) return SP_NOT_OK;
    else return SP_OK;
}
