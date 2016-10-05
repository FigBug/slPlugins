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
    sp_pshift *pshift;
    sp_diskin *diskin;
} UserData;

void process(sp_data *sp, void *udata) {
    UserData *ud = udata;
    SPFLOAT diskin = 0, pshift = 0;
    sp_diskin_compute(sp, ud->diskin, NULL, &diskin);
    sp_pshift_compute(sp, ud->pshift, &diskin, &pshift);
    sp->out[0] = pshift;
}

int main() {
    srand(1234567);
    UserData ud;
    sp_data *sp;
    sp_create(&sp);

    sp_pshift_create(&ud.pshift);
    sp_diskin_create(&ud.diskin);    

    sp_pshift_init(sp, ud.pshift);
    *ud.pshift->shift = 7;
    *ud.pshift->window = 500;
    /* half window size is smoothest sounding */
    *ud.pshift->xfade = 250;
    sp_diskin_init(sp, ud.diskin, "oneart.wav");

    sp->len = 44100 * 5;
    sp_process(sp, &ud, process);

    sp_pshift_destroy(&ud.pshift);
    sp_diskin_destroy(&ud.diskin);

    sp_destroy(&sp);
    return 0;
}
