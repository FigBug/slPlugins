#include "soundpipe.h"
#include "md5.h"
#include "tap.h"
#include "test.h"

typedef struct {
    sp_paulstretch *paulstretch;
    sp_ftbl *ft; 
} UserData;

int t_paulstretch(sp_test *tst, sp_data *sp, const char *hash) 
{
    uint32_t n;
    int fail = 0;
    SPFLOAT paulstretch = 0;

    sp_srand(sp, 1234567);
    UserData ud;

    sp_paulstretch_create(&ud.paulstretch);
    sp_ftbl_loadfile(sp, &ud.ft, SAMPDIR "oneart.wav");

    sp_paulstretch_init(sp, ud.paulstretch, ud.ft, 1.0, 10);


    for(n = 0; n < tst->size; n++) {
        paulstretch = 0;
        sp_paulstretch_compute(sp, ud.paulstretch, NULL, &paulstretch);
        sp->out[0] = paulstretch;
        sp_test_add_sample(tst, sp->out[0]);
    }

    fail = sp_test_verify(tst, hash);

    sp_paulstretch_destroy(&ud.paulstretch);
    sp_ftbl_destroy(&ud.ft);

    if(fail) return SP_NOT_OK;
    else return SP_OK;
}
