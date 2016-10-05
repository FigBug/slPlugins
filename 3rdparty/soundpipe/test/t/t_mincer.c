#include "soundpipe.h"
#include "md5.h"
#include "tap.h"
#include "test.h"

typedef struct {
    sp_mincer *mincer;
    sp_ftbl *ft; 
    sp_randi *randi;
} UserData;

int t_mincer(sp_test *tst, sp_data *sp, const char *hash) 
{
    uint32_t n;
    int fail = 0;
    SPFLOAT mincer = 0, randi = 0;

    UserData ud;
    sp_srand(sp, 1234567);

    sp_mincer_create(&ud.mincer);
    /* allocates loads an audio file into a ftable */
    sp_ftbl_loadfile(sp, &ud.ft, SAMPDIR "oneart.wav");

    sp_randi_create(&ud.randi);

    sp_mincer_init(sp, ud.mincer, ud.ft);
    sp_randi_init(sp, ud.randi);
    ud.randi->min = 0;
    ud.randi->max = 5;
    ud.randi->cps = 1;

    for(n = 0; n < tst->size; n++) {
        mincer = 0; randi = 0;
        
        sp_randi_compute(sp, ud.randi, NULL, &randi);
        ud.mincer->time = randi;
        ud.mincer->amp = 1;
        ud.mincer->pitch = 1;
        sp_mincer_compute(sp, ud.mincer, NULL, &mincer);
        sp_test_add_sample(tst, mincer);
    }

    fail = sp_test_verify(tst, hash);

    sp_mincer_destroy(&ud.mincer);
    sp_ftbl_destroy(&ud.ft);
    sp_randi_destroy(&ud.randi);

    if(fail) return SP_NOT_OK;
    else return SP_OK;
}
