#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "soundpipe.h"

typedef struct {
    sp_tenv2 *tenv2;
    sp_osc *osc;
    sp_ftbl *ft; 
} UserData;

void process(sp_data *sp, void *udata) {
    UserData *ud = udata;
    SPFLOAT osc = 0, tenv2 = 0, tick = 0;

    if(sp->pos == 0 || sp->pos == sp->sr * 1) tick = 1;

    sp_osc_compute(sp, ud->osc, NULL, &osc);
    sp_tenv2_compute(sp, ud->tenv2, &tick, &tenv2);
    sp->out[0] = osc * tenv2;
}

int main() {
    srand(1234567);
    UserData ud;
    sp_data *sp;
    sp_create(&sp);

    sp_tenv2_create(&ud.tenv2);
    sp_osc_create(&ud.osc);
    sp_ftbl_create(sp, &ud.ft, 2048);

    sp_tenv2_init(sp, ud.tenv2);
    ud.tenv2->atk = 3;
    ud.tenv2->rel = 1;
    sp_gen_sine(sp, ud.ft);
    sp_osc_init(sp, ud.osc, ud.ft, 0);
    ud.osc->amp = 0.6;

    sp->len = 44100 * 5;
    sp_process(sp, &ud, process);

    sp_tenv2_destroy(&ud.tenv2);
    sp_ftbl_destroy(&ud.ft);
    sp_osc_destroy(&ud.osc);

    sp_destroy(&sp);
    return 0;
}
