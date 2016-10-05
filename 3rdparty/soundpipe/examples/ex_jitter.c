#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "soundpipe.h"

typedef struct {
    sp_fosc *osc;
    sp_ftbl *ft; 
    sp_jitter *jit;
} UserData;

void process(sp_data *sp, void *udata) {
    UserData *ud = udata;
    SPFLOAT jit = 0;
    sp_jitter_compute(sp, ud->jit, NULL, &jit);
    ud->osc->freq = sp_midi2cps(60 + jit);
    sp_fosc_compute(sp, ud->osc, NULL, &sp->out[0]);
}

int main() {
    srand(time(NULL));
    UserData ud;
    sp_data *sp;
    sp_create(&sp);
    sp_ftbl_create(sp, &ud.ft, 2048);
    sp_fosc_create(&ud.osc);
    sp_jitter_create(&ud.jit);   

    sp_gen_sine(sp, ud.ft);
    sp_fosc_init(sp, ud.osc, ud.ft);
    sp_jitter_init(sp, ud.jit);
    ud.jit->cpsMin = 0.5;
    ud.jit->cpsMax = 4;
    ud.jit->amp = 3;  
    sp->len = 44100 * 5;
    sp_process(sp, &ud, process);

    sp_ftbl_destroy(&ud.ft);
    sp_fosc_destroy(&ud.osc);
    sp_jitter_destroy(&ud.jit);

    sp_destroy(&sp);
    return 0;
}
