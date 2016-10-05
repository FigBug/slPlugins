#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "soundpipe.h"

typedef struct {
    sp_sdelay *sdelay;
    sp_lpf18 *filt1;
    sp_lpf18 *filt2;
    sp_metro *met;
} UserData;

void process(sp_data *sp, void *udata) {
    UserData *ud = udata;
    SPFLOAT met = 0, sdelay = 0;
    SPFLOAT filt1 = 0, filt2 = 0;
    sp_metro_compute(sp, ud->met, NULL, &met);
    sp_sdelay_compute(sp, ud->sdelay, &met, &sdelay);

    sp_lpf18_compute(sp, ud->filt1, &met, &filt1);
    sp_lpf18_compute(sp, ud->filt2, &sdelay, &filt2);

    sp->out[0] = filt1 + filt2;
}

int main() {
    srand(1234567);
    UserData ud;
    sp_data *sp;
    sp_create(&sp);

    sp_sdelay_create(&ud.sdelay);
    sp_metro_create(&ud.met);
    sp_lpf18_create(&ud.filt1);
    sp_lpf18_create(&ud.filt2);

    sp_sdelay_init(sp, ud.sdelay, 2000);
    sp_metro_init(sp, ud.met);

    ud.met->freq = 2;

    sp_lpf18_init(sp, ud.filt1);
    ud.filt1->cutoff = 4000;
    ud.filt1->res = 0.8;

    sp_lpf18_init(sp, ud.filt2);
    ud.filt2->cutoff = 500;
    ud.filt2->res = 0.8;

    sp->len = 44100 * 5;
    sp_process(sp, &ud, process);

    sp_sdelay_destroy(&ud.sdelay);
    sp_metro_destroy(&ud.met);
    sp_lpf18_destroy(&ud.filt1);
    sp_lpf18_destroy(&ud.filt2);

    sp_destroy(&sp);
    return 0;
}
