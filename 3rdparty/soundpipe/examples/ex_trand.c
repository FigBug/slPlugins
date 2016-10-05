
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "soundpipe.h"

typedef struct {
    sp_metro *met;
    sp_trand *trand;
    sp_osc *osc;
    sp_ftbl *ft; 
} UserData;

void process(sp_data *sp, void *udata) {
    UserData *ud = udata;
    SPFLOAT osc = 0, trand = 0, met = 0;
    sp_metro_compute(sp, ud->met, NULL, &met);
    sp_trand_compute(sp, ud->trand, &met, &trand);
    ud->osc->freq = trand;
    sp_osc_compute(sp, ud->osc, NULL, &osc);
    sp->out[0] = osc;
}

int main() {
    UserData ud;
    sp_data *sp;
    sp_create(&sp);
    sp_srand(sp, 1234567);

    sp_metro_create(&ud.met);
    sp_trand_create(&ud.trand);
    sp_osc_create(&ud.osc);
    sp_ftbl_create(sp, &ud.ft, 2048);

    sp_metro_init(sp, ud.met);
    ud.met->freq = 20;
    sp_trand_init(sp, ud.trand);
    ud.trand->min = 40;
    ud.trand->max = 1000;

    sp_gen_sine(sp, ud.ft);
    sp_osc_init(sp, ud.osc, ud.ft, 0);

    sp->len = 44100 * 5;
    sp_process(sp, &ud, process);

    sp_trand_destroy(&ud.trand);
    sp_ftbl_destroy(&ud.ft);
    sp_osc_destroy(&ud.osc);
    sp_metro_destroy(&ud.met);

    sp_destroy(&sp);
    return 0;
}
