#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "soundpipe.h"

typedef struct {
    sp_tadsr *tadsr;
    sp_osc *osc;
    sp_ftbl *ft; 
} UserData;

void process(sp_data *sp, void *udata) {
    UserData *ud = udata;
    SPFLOAT osc = 0, tadsr = 0, trig = 0;
    if(sp->pos == 0 || sp->pos == sp->sr * 2) trig = 1;
    sp_tadsr_compute(sp, ud->tadsr, &trig, &tadsr);
    sp_osc_compute(sp, ud->osc, NULL, &osc);
    sp->out[0] = osc * tadsr;
}

int main() {
    srand(1234567);
    UserData ud;
    sp_data *sp;
    sp_create(&sp);

    sp_tadsr_create(&ud.tadsr);
    sp_osc_create(&ud.osc);
    sp_ftbl_create(sp, &ud.ft, 2048);

    sp_tadsr_init(sp, ud.tadsr);
    ud.tadsr->atk = 0.1;
    ud.tadsr->dec = 0.2;
    ud.tadsr->sus = 0.3;
    ud.tadsr->rel = 0.1;
    sp_gen_sine(sp, ud.ft);
    sp_osc_init(sp, ud.osc, ud.ft, 0);

    sp->len = 44100 * 5;
    sp_process(sp, &ud, process);

    sp_tadsr_destroy(&ud.tadsr);
    sp_ftbl_destroy(&ud.ft);
    sp_osc_destroy(&ud.osc);

    sp_destroy(&sp);
    return 0;
}
