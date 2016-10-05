#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "soundpipe.h"
#include "md5.h"
#include "tap.h"
#include "test.h"

int t_blsaw(sp_test *tst, sp_data *sp, const char *hash) {
    uint32_t n;
    int fail = 0;
    SPFLOAT blsaw;
    sp_blsaw *blsaw_d;

    sp_blsaw_create(&blsaw_d);

    sp_blsaw_init(sp, blsaw_d);
    *blsaw_d->freq = 500;
    sp->len = 44100 * 5;

    for(n = 0; n < tst->size; n++) {
        blsaw = 0;
        sp_blsaw_compute(sp, blsaw_d, NULL, &blsaw);
        sp_out(sp, 0, blsaw);
        sp_test_add_sample(tst, sp->out[0]);
    }

    fail = sp_test_verify(tst, hash);

    sp_blsaw_destroy(&blsaw_d);

    if(fail) return SP_NOT_OK;
    else return SP_OK;
}

