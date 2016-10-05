#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "soundpipe.h"

typedef struct {
    sp_tseg *tseg;
    sp_osc *osc;
    sp_ftbl *ft; 
} UserData;

void process(sp_data *sp, void *udata) {
    UserData *ud = udata;
    SPFLOAT osc = 0, tseg = 0;
    SPFLOAT trig = (sp->pos == 0);
    sp_tseg_compute(sp, ud->tseg, &trig, &tseg);
    ud->osc->freq = 100 + (tseg * 1000);
    sp_osc_compute(sp, ud->osc, NULL, &osc);
    sp->out[0] = osc;
}

int main() {
    srand(1234567);
    UserData ud;
    sp_data *sp;
    sp_create(&sp);

    sp_tseg_create(&ud.tseg);
    sp_osc_create(&ud.osc);
    sp_ftbl_create(sp, &ud.ft, 2048);

    sp_tseg_init(sp, ud.tseg, 0.0001);
    ud.tseg->end = 1.0;
    ud.tseg->type = 3.0;
    ud.tseg->dur = 4.0;

    sp_gen_sine(sp, ud.ft);
    sp_osc_init(sp, ud.osc, ud.ft, 0);

    sp->len = 44100 * 5;
    sp_process(sp, &ud, process);

    sp_tseg_destroy(&ud.tseg);
    sp_ftbl_destroy(&ud.ft);
    sp_osc_destroy(&ud.osc);

    sp_destroy(&sp);
    return 0;
}
