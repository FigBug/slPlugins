#include "soundpipe.h"
#include "md5.h"
#include "tap.h"
#include "test.h"

typedef struct {
    sp_expon *line;
    sp_osc *osc;
    sp_ftbl *ft;
} UserData;

int t_expon(sp_test *tst, sp_data *sp, const char *hash) 
{

    sp_srand(sp, 1234567);
    uint32_t n;
    int fail = 0;
    SPFLOAT osc = 0, line = 0;
    SPFLOAT tick = 0;
    UserData ud;

    sp_expon_create(&ud.line);
    sp_osc_create(&ud.osc);
    sp_ftbl_create(sp, &ud.ft, 2048);

    sp_expon_init(sp, ud.line);
    ud.line->a = 100;
    ud.line->dur = 3;
    ud.line->b = 400;
    sp_gen_sine(sp, ud.ft);
    sp_osc_init(sp, ud.osc, ud.ft, 0);

    sp->len = 44100 * 5;

    for(n = 0; n < tst->size; n++) {
        if(n == 0) tick = 1; else tick = 0;
        sp_expon_compute(sp, ud.line, &tick, &line);
        ud.osc->freq = line;
        sp_osc_compute(sp, ud.osc, NULL, &osc);
        sp->out[0] = osc;

        sp_test_add_sample(tst, sp->out[0]);
    }

    fail = sp_test_verify(tst, hash);

    sp_expon_destroy(&ud.line);
    sp_ftbl_destroy(&ud.ft);
    sp_osc_destroy(&ud.osc);
 
    if(fail) return SP_NOT_OK;
    else return SP_OK;
}
