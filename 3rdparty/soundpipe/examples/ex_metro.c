#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "soundpipe.h"

typedef struct {
    sp_osc *osc;
    sp_ftbl *ft;
    sp_tenv *tenv;
    sp_metro *met;
    sp_randi *rand;
    SPFLOAT freq;
} UserData;

void write_osc(sp_data *sp, void *udata) {
    UserData *ud = udata;
    SPFLOAT osc = 0, trig = 0, tenv = 0;
    sp_randi_compute(sp, ud->rand, NULL, &ud->met->freq);
    sp_metro_compute(sp, ud->met, NULL, &trig);
    sp_tenv_compute(sp, ud->tenv, &trig, &tenv);
    sp_osc_compute(sp, ud->osc, NULL, &osc);
    sp->out[0] = tenv * osc;
}

int main() {
    UserData ud;
    SPFLOAT *freqp = &ud.freq;
    ud.freq = 400;
    sp_data *sp;
    sp_create(&sp);
    sp_srand(sp, 12345);

    sp_randi_create(&ud.rand);
    sp_metro_create(&ud.met);
    sp_tenv_create(&ud.tenv);
    sp_ftbl_create(sp, &ud.ft, 2048);
    sp_osc_create(&ud.osc);

    sp_randi_init(sp, ud.rand);
    ud.rand->min = 2.0;
    ud.rand->max= 15.0;
    sp_metro_init(sp, ud.met);
    sp_tenv_init(sp, ud.tenv);
    ud.tenv->atk = 0.005;
    ud.tenv->hold = 0.01;
    ud.tenv->rel = 0.003;
    sp_gen_sine(sp, ud.ft);
    sp_osc_init(sp, ud.osc, ud.ft, 0);
    ud.osc->freq = *freqp;
    sp->len = 44100 * 5;
    sp_process(sp, &ud, write_osc);

    sp_randi_destroy(&ud.rand);
    sp_metro_destroy(&ud.met);
    sp_ftbl_destroy(&ud.ft);
    sp_osc_destroy(&ud.osc);
    sp_tenv_destroy(&ud.tenv);
    sp_destroy(&sp);
    return 0;
}
