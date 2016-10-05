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
    SPFLOAT in = 0, out1 = 0, out2 = 0;

    sp_pan2 *unit[NUM];

    for(u = 0; u < NUM; u++) { 
        sp_pan2_create(&unit[u]);
        sp_pan2_init(sp, unit[u]);
        unit[u]->type  = 1;
    }

    for(t = 0; t < sp->len; t++) {
        for(u = 0; u < NUM; u++) sp_pan2_compute(sp, unit[u], &in, 
                &out1, &out2);
    }

    for(u = 0; u < NUM; u++) sp_pan2_destroy(&unit[u]);

    sp_destroy(&sp);
    return 0;
}

