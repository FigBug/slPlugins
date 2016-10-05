#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "soundpipe.h"

typedef struct {
    sp_biscale *biscale;
    sp_osc *osc;
    sp_ftbl *ft;
} UserData;

void process(sp_data *sp, void *udata) {
    UserData *ud = udata;
    SPFLOAT osc = 0, biscale = 0;
    /* constant set to 1, when scaled, it becomes 440 */
    SPFLOAT val = 1;
    sp_biscale_compute(sp, ud->biscale, &val, &biscale);
    ud->osc->freq = biscale;
    sp_osc_compute(sp, ud->osc, NULL, &osc);
    sp->out[0] = osc;
}

int main() {
    srand(1234567);
    UserData ud;
    sp_data *sp;
    sp_create(&sp);

    sp_biscale_create(&ud.biscale);
    sp_osc_create(&ud.osc);
    sp_ftbl_create(sp, &ud.ft, 2048);

    sp_biscale_init(sp, ud.biscale);
    ud.biscale->min = 0;
    ud.biscale->max = 880;
    sp_gen_sine(sp, ud.ft);
    sp_osc_init(sp, ud.osc, ud.ft, 0);
    ud.osc->amp = 0.1;

    sp->len = 44100 * 5;
    sp_process(sp, &ud, process);

    sp_biscale_destroy(&ud.biscale);
    sp_ftbl_destroy(&ud.ft);
    sp_osc_destroy(&ud.osc);

    sp_destroy(&sp);
    return 0;
}
