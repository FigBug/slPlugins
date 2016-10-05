#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "soundpipe.h"
#include "md5.h"
#include "tap.h"
#include "test.h"

int t_blsquare(sp_test *tst, sp_data *sp, const char *hash) {
    uint32_t n;
    int fail = 0;
    SPFLOAT blsquare;
    sp_blsquare *blsquare_d;

    sp_blsquare_create(&blsquare_d);

    sp_blsquare_init(sp, blsquare_d);
    *blsquare_d->freq = 500;
    *blsquare_d->width = 0.4;
    sp->len = 44100 * 5;

    for(n = 0; n < tst->size; n++) {
        blsquare = 0;
        sp_blsquare_compute(sp, blsquare_d, NULL, &blsquare);
        sp_out(sp, 0, blsquare);
        sp_test_add_sample(tst, sp->out[0]);
    }

    fail = sp_test_verify(tst, hash);

    sp_blsquare_destroy(&blsquare_d);

    if(fail) return SP_NOT_OK;
    else return SP_OK;
}

