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
    sp_metro *met;
    sp_incr *incr;
    sp_osc *osc;
    sp_ftbl *ft; 
} UserData;

void process(sp_data *sp, void *udata) 
{
    UserData *ud = udata;
    SPFLOAT osc = 0, incr = 0, met = 0;
    sp_metro_compute(sp, ud->met, NULL, &met);
    sp_incr_compute(sp, ud->incr, &met, &incr);
    ud->osc->freq = incr;
    sp_osc_compute(sp, ud->osc, NULL, &osc);
    sp->out[0] = osc;
}

int main() 
{
    UserData ud;
    sp_data *sp;
    sp_create(&sp);
    sp_srand(sp, 1234567);

    sp_metro_create(&ud.met);
    sp_incr_create(&ud.incr);
    sp_osc_create(&ud.osc);
    sp_ftbl_create(sp, &ud.ft, 2048);

    sp_metro_init(sp, ud.met);
    ud.met->freq = 3;
    sp_incr_init(sp, ud.incr, 300);
    ud.incr->min = 300;
    ud.incr->max = 900;
    ud.incr->step = 30;
    sp_gen_sine(sp, ud.ft);
    sp_osc_init(sp, ud.osc, ud.ft, 0);
    ud.osc->amp = 0.5;

    sp->len = 44100 * 5;
    sp_process(sp, &ud, process);

    sp_incr_destroy(&ud.incr);
    sp_ftbl_destroy(&ud.ft);
    sp_osc_destroy(&ud.osc);
    sp_metro_destroy(&ud.met);

    sp_destroy(&sp);
    return 0;
}
