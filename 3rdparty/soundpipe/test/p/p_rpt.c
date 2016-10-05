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
    SPFLOAT in = 0, out = 0, trig = 0;

    sp_rpt *unit[NUM];

    for(u = 0; u < NUM; u++) { 
        sp_rpt_create(&unit[u]);
        sp_rpt_init(sp, unit[u], 2.0);
    }

    for(t = 0; t < sp->len; t++) {
        for(u = 0; u < NUM; u++) sp_rpt_compute(sp, unit[u], &trig, &in, &out);
    }

    for(u = 0; u < NUM; u++) sp_rpt_destroy(&unit[u]);

    sp_destroy(&sp);
    return 0;
}

