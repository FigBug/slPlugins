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

    sp_compressor *unit[NUM];

    for(u = 0; u < NUM; u++) { 
        sp_compressor_create(&unit[u]);
        sp_compressor_init(sp, unit[u]);
    }

    for(t = 0; t < sp->len; t++) {
        for(u = 0; u < NUM; u++) sp_compressor_compute(sp, unit[u], &in, &out);
    }

    for(u = 0; u < NUM; u++) sp_compressor_destroy(&unit[u]);

    sp_destroy(&sp);
    return 0;
}

