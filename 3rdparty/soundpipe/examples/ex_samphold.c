#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "soundpipe.h"

typedef struct {
    sp_samphold *samphold;
    sp_osc *osc;
    sp_ftbl *ft;
    sp_metro *met;
    sp_noise *noise;
} UserData;

void process(sp_data *sp, void *udata) {
    UserData *ud = udata;
    SPFLOAT osc = 0, samphold = 0, met = 0, noise = 0;
    sp_metro_compute(sp, ud->met, NULL, &met);
    sp_noise_compute(sp, ud->noise, NULL, &noise);
    sp_samphold_compute(sp, ud->samphold, &met, &noise, &samphold);
    ud->osc->freq = 200 + (samphold + 1) * 300;
    sp_osc_compute(sp, ud->osc, NULL, &osc);
    sp->out[0] = osc;
}

int main() {
    srand(1234567);
    UserData ud;
    sp_data *sp;
    sp_create(&sp);

    sp_samphold_create(&ud.samphold);
    sp_osc_create(&ud.osc);
    sp_ftbl_create(sp, &ud.ft, 2048);
    sp_metro_create(&ud.met);
    sp_noise_create(&ud.noise);

    sp_samphold_init(sp, ud.samphold);
    sp_gen_sine(sp, ud.ft);
    sp_osc_init(sp, ud.osc, ud.ft, 0);
    sp_metro_init(sp, ud.met);
    ud.met->freq = 5;
    sp_noise_init(sp, ud.noise);
    ud.noise->amp = 1;
    sp->len = 44100 * 5;
    sp_process(sp, &ud, process);

    sp_samphold_destroy(&ud.samphold);
    sp_ftbl_destroy(&ud.ft);
    sp_osc_destroy(&ud.osc);
    sp_metro_destroy(&ud.met);
    sp_noise_destroy(&ud.noise);

    sp_destroy(&sp);
    return 0;
}
