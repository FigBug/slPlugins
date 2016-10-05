#include <stdlib.h>
#include <stdio.h>
#include "soundpipe.h"
#include "config.h"

int main() {
    sp_data *sp;
    sp_create(&sp);
    sp_srand(sp, 12345);
    sp->sr = SR;
    sp->len = sp->sr * LEN;
    uint32_t t, u;
    SPFLOAT in = 0, out = 0;

    sp_tseg *unit[NUM];

    for(u = 0; u < NUM; u++) { 
        sp_tseg_create(&unit[u]);
        sp_tseg_init(sp, unit[u], 0);
    }

    for(t = 0; t < sp->len; t++) {
        for(u = 0; u < NUM; u++) sp_tseg_compute(sp, unit[u], &in, &out);
    }

    for(u = 0; u < NUM; u++) sp_tseg_destroy(&unit[u]);

    sp_destroy(&sp);
    return 0;
}

