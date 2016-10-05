#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "soundpipe.h"

typedef struct {
    sp_tgate *tgate;
    sp_osc *osc;
    sp_ftbl *ft; 
    sp_dmetro *dm;
} UserData;

void process(sp_data *sp, void *udata) {
    UserData *ud = udata;
    SPFLOAT osc = 0, tgate = 0;
    SPFLOAT dm = 0;
    sp_dmetro_compute(sp, ud->dm, NULL, &dm);
    sp_tgate_compute(sp, ud->tgate, &dm, &tgate);
    sp_osc_compute(sp, ud->osc, NULL, &osc);
    sp->out[0] = osc * tgate;
}

int main() {
    UserData ud;
    sp_data *sp;
    sp_create(&sp);
    sp_srand(sp, 1234567);

    sp_tgate_create(&ud.tgate);
    sp_osc_create(&ud.osc);
    sp_ftbl_create(sp, &ud.ft, 2048);
    sp_dmetro_create(&ud.dm);

    sp_tgate_init(sp, ud.tgate);
    ud.tgate->time = 0.2;
    sp_gen_sine(sp, ud.ft);
    sp_osc_init(sp, ud.osc, ud.ft, 0);
    ud.osc->freq = 1000;
    sp_dmetro_init(sp, ud.dm);
    ud.dm->time = 0.5;

    sp->len = 44100 * 5;
    sp_process(sp, &ud, process);

    sp_tgate_destroy(&ud.tgate);
    sp_ftbl_destroy(&ud.ft);
    sp_osc_destroy(&ud.osc);
    sp_dmetro_destroy(&ud.dm);

    sp_destroy(&sp);
    return 0;
}
