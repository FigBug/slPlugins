#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "soundpipe.h"

typedef struct {
    sp_tabread *tr;
    sp_ftbl *ft;
    sp_phasor *phasor;
} UserData;

void process(sp_data *sp, void *udata) {
    UserData *ud = udata;
    SPFLOAT tab = 0.0, phasor = 0.0;
    sp_phasor_compute(sp, ud->phasor, NULL, &phasor);
    ud->tr->index = phasor;
    sp_tabread_compute(sp, ud->tr, NULL, &tab);
    sp->out[0] = tab;
}

int main() {
    srand(time(NULL));
    UserData ud;
    sp_data *sp;
    sp_create(&sp);

    sp_tabread_create(&ud.tr);

    sp_phasor_create(&ud.phasor);

    sp_ftbl_create(sp, &ud.ft, 395393);
    sp_gen_file(sp, ud.ft, "oneart.wav");

    sp_tabread_init(sp, ud.tr, ud.ft, 1);

    /* since mode = 1, offset 5% into file */
    ud.tr->offset = 0.05;
    /* no wraparound */
    ud.tr->wrap = 0;

    sp_phasor_init(sp, ud.phasor, 0);
    /* set playback rate to half speed, or 1/(t * 2) */
    ud.phasor->freq = 1 / (8.97 * 2);
    sp->len = 44100 * 5;
    sp_process(sp, &ud, process);

    sp_phasor_destroy(&ud.phasor);
    sp_tabread_destroy(&ud.tr);
    sp_ftbl_destroy(&ud.ft);
    sp_destroy(&sp);
    return 0;
}
