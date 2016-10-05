#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "soundpipe.h"

typedef struct {
    sp_thresh *thresh;
    sp_osc *osc, *lfo;
    sp_ftbl *ft; 
    sp_tenv *tenv;
} UserData;

void process(sp_data *sp, void *udata) {
    UserData *ud = udata;
    SPFLOAT osc = 0, thresh = 0, lfo = 0, tenv = 0;
    sp_osc_compute(sp, ud->lfo, NULL, &lfo);
    /* Create a tick for every positive zero crossing */
    sp_thresh_compute(sp, ud->thresh, &lfo, &thresh);
    sp_tenv_compute(sp, ud->tenv, &thresh, &tenv);
    sp_osc_compute(sp, ud->osc, NULL, &osc);
    sp->out[0] = osc * tenv;
}

int main() {
    srand(1234567);
    UserData ud;
    sp_data *sp;
    sp_create(&sp);

    sp_thresh_create(&ud.thresh);
    sp_osc_create(&ud.osc);
    sp_osc_create(&ud.lfo);
    sp_tenv_create(&ud.tenv);
    sp_ftbl_create(sp, &ud.ft, 2048);

    sp_thresh_init(sp, ud.thresh);
    ud.thresh->mode = 1;
    sp_gen_sine(sp, ud.ft);
    sp_osc_init(sp, ud.osc, ud.ft, 0);

    sp_osc_init(sp, ud.lfo, ud.ft, 0);
    ud.lfo->freq = 3;
    ud.lfo->amp = 1;

    sp_tenv_init(sp, ud.tenv);
    ud.tenv->atk = 0.01;
    ud.tenv->hold = 0.01;
    ud.tenv->rel = 0.01;

    sp->len = 44100 * 5;
    sp_process(sp, &ud, process);

    sp_thresh_destroy(&ud.thresh);
    sp_ftbl_destroy(&ud.ft);
    sp_osc_destroy(&ud.osc);
    sp_osc_destroy(&ud.lfo);
    sp_tenv_destroy(&ud.tenv);

    sp_destroy(&sp);
    return 0;
}
