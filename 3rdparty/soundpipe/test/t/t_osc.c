#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "soundpipe.h"
#include "md5.h"
#include "tap.h"
#include "test.h"

int t_osc(sp_test *tst, sp_data *sp, const char *hash) {
    uint32_t n;
    int fail = 0;
    SPFLOAT osc;
    sp_osc *osc_d;
    sp_ftbl *ft_d; 

    sp_ftbl_create(sp, &ft_d, 2048);
    sp_osc_create(&osc_d);
    
    sp_gen_sine(sp, ft_d);
    sp_osc_init(sp, osc_d, ft_d, 0);
    osc_d->freq = 500;
    sp->len = 44100 * 5;
    
    for(n = 0; n < tst->size; n++) {
        osc = 0;
        sp_osc_compute(sp, osc_d, NULL, &osc);
        sp_out(sp, 0, osc);
        sp_test_add_sample(tst, sp->out[0]);
    }

    fail = sp_test_verify(tst, hash);

    sp_ftbl_destroy(&ft_d);
    sp_osc_destroy(&osc_d);

    if(fail) return SP_NOT_OK;
    else return SP_OK;
}

