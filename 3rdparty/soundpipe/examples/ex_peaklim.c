/*
 * This is a dummy example.
 * Please implement a small and simple working example of your module, and then
 * remove this header.
 * Don't be clever.
 * Bonus points for musicality. 
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "soundpipe.h"

typedef struct {
    sp_peaklim *peaklim;
    sp_diskin *diskin;
} UserData;

void process(sp_data *sp, void *udata) {
    UserData *ud = udata;
    SPFLOAT diskin = 0, peaklim = 0;
    sp_diskin_compute(sp, ud->diskin, NULL, &diskin);
    sp_peaklim_compute(sp, ud->peaklim, &diskin, &peaklim);
    sp->out[0] = peaklim;
}

int main() {
    UserData ud;
    sp_data *sp;
    sp_create(&sp);
    sp_srand(sp, 1234567);

    sp_peaklim_create(&ud.peaklim);
    sp_diskin_create(&ud.diskin);

    sp_peaklim_init(sp, ud.peaklim);
    ud.peaklim->atk = 0.1;
    ud.peaklim->rel = 0.1;
    ud.peaklim->thresh = -30;
    sp_diskin_init(sp, ud.diskin, "oneart.wav");


    sp->len = 44100 * 5;
    sp_process(sp, &ud, process);

    sp_peaklim_destroy(&ud.peaklim);
    sp_diskin_destroy(&ud.diskin);

    sp_destroy(&sp);
    return 0;
}
