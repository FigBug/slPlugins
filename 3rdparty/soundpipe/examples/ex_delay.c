#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "soundpipe.h"

typedef struct {
    sp_delay *delay;
    sp_osc *osc;
    sp_metro *met;
    sp_tenv *tenv;
    sp_ftbl *ft;
} UserData;

void process(sp_data *sp, void *udata) {
    UserData *ud = udata;
    SPFLOAT osc = 0, delay = 0, met = 0, tenv = 0;
    sp_metro_compute(sp, ud->met, NULL, &met);
    sp_tenv_compute(sp, ud->tenv, &met, &tenv);
    if(met) {
        ud->osc->freq = 100 + rand() % 500;
    }
    sp_osc_compute(sp, ud->osc, NULL, &osc);
    osc *= tenv;
    sp_delay_compute(sp, ud->delay, &osc, &delay);
    sp->out[0] = osc + delay;
}

int main() {
    srand(1234567);
    UserData ud;
    sp_data *sp;
    sp_create(&sp);

    sp_delay_create(&ud.delay);
    sp_osc_create(&ud.osc);
    sp_ftbl_create(sp, &ud.ft, 2048);
    sp_metro_create(&ud.met);
    sp_tenv_create(&ud.tenv);

    sp_delay_init(sp, ud.delay, 0.75 * 0.5);
    ud.delay->feedback = 0.5;
    sp_gen_sine(sp, ud.ft);
    sp_osc_init(sp, ud.osc, ud.ft, 0);
    ud.osc->amp = 0.5;
    sp_metro_init(sp, ud.met);
    ud.met->freq = 1;
    sp_tenv_init(sp, ud.tenv);
    ud.tenv->atk = 0.005;
    ud.tenv->hold = 0.1;
    ud.tenv->rel =  0.1;


    sp->len = 44100 * 5;
    sp_process(sp, &ud, process);

    sp_delay_destroy(&ud.delay);
    sp_ftbl_destroy(&ud.ft);
    sp_osc_destroy(&ud.osc);
    sp_metro_destroy(&ud.met);
    sp_tenv_destroy(&ud.tenv);

    sp_destroy(&sp);
    return 0;
}
