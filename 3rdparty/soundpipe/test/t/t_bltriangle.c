#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "soundpipe.h"
#include "md5.h"
#include "tap.h"
#include "test.h"

int t_bltriangle(sp_test *tst, sp_data *sp, const char *hash) {
    uint32_t n;
    int fail = 0;
    SPFLOAT bltriangle;
    sp_bltriangle *bltriangle_d;

    sp_bltriangle_create(&bltriangle_d);

    sp_bltriangle_init(sp, bltriangle_d);
    *bltriangle_d->freq = 500;
    sp->len = 44100 * 5;

    for(n = 0; n < tst->size; n++) {
        bltriangle = 0;
        sp_bltriangle_compute(sp, bltriangle_d, NULL, &bltriangle);
        sp_out(sp, 0, bltriangle);
        sp_test_add_sample(tst, sp->out[0]);
    }

    fail = sp_test_verify(tst, hash);

    sp_bltriangle_destroy(&bltriangle_d);

    if(fail) return SP_NOT_OK;
    else return SP_OK;
}

