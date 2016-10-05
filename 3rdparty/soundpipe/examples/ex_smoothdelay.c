#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "soundpipe.h"

typedef struct {
    sp_smoothdelay *smoothdelay;
    sp_osc *osc;
    sp_ftbl *ft; 
    sp_diskin *diskin;
} UserData;

void process(sp_data *sp, void *udata) {
    UserData *ud = udata;
    SPFLOAT osc = 0, smoothdelay = 0, diskin = 0;
    sp_diskin_compute(sp, ud->diskin, NULL, &diskin);
    sp_osc_compute(sp, ud->osc, NULL, &osc);
    osc = 0.1 + (0.5 * (1 + osc)) * 0.8;
    ud->smoothdelay->del = osc;
    sp_smoothdelay_compute(sp, ud->smoothdelay, &diskin, &smoothdelay);
    sp->out[0] = diskin + smoothdelay;
}

int main() {
    UserData ud;
    sp_data *sp;
    sp_create(&sp);
    sp_srand(sp, 1234567);

    sp_smoothdelay_create(&ud.smoothdelay);
    sp_osc_create(&ud.osc);
    sp_ftbl_create(sp, &ud.ft, 2048);
    sp_diskin_create(&ud.diskin);

    sp_smoothdelay_init(sp, ud.smoothdelay, 1.0, 1024);
    ud.smoothdelay->feedback = 0.5;
    sp_gen_sine(sp, ud.ft);
    sp_osc_init(sp, ud.osc, ud.ft, 0);
    ud.osc->freq = 0.2;
    ud.osc->amp = 1.0;
    sp_diskin_init(sp, ud.diskin, "oneart.wav");

    sp->len = 44100 * 10;
    sp_process(sp, &ud, process);

    sp_smoothdelay_destroy(&ud.smoothdelay);
    sp_ftbl_destroy(&ud.ft);
    sp_osc_destroy(&ud.osc);
    sp_diskin_destroy(&ud.diskin);

    sp_destroy(&sp);
    return 0;
}
