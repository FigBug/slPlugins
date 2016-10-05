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
    sp_oscmorph *oscmorph;
    sp_ftbl *wt1;
    sp_osc *osc;
    sp_ftbl *ft; 
} UserData;

void process(sp_data *sp, void *udata) {
    UserData *ud = udata;
    SPFLOAT osc = 0, oscmorph = 0;
    sp_osc_compute(sp, ud->osc, NULL, &osc);
    osc = (1 + osc) * 0.5;
    ud->oscmorph->wtpos = osc;
    sp_oscmorph_compute(sp, ud->oscmorph, NULL, &oscmorph);
    sp->out[0] = oscmorph;
}

int main() {
    srand(1234567);
    UserData ud;
    sp_data *sp;
    sp_create(&sp);

    sp_oscmorph_create(&ud.oscmorph);
    sp_osc_create(&ud.osc);
    sp_ftbl_create(sp, &ud.ft, 2048);
    sp_ftbl_create(sp, &ud.wt1, 2048);

    sp_gen_line(sp, ud.wt1, "0 1 2048 -1");
    sp_gen_sine(sp, ud.ft);

    sp_ftbl *ft_array[] = {ud.wt1, ud.ft};
    sp_oscmorph_init(sp, ud.oscmorph, ft_array, 2, 0);
    sp_osc_init(sp, ud.osc, ud.ft, 0);
    ud.osc->freq = 1;
    ud.osc->amp = 1;

    sp->len = 44100 * 5;
    sp_process(sp, &ud, process);

    sp_oscmorph_destroy(&ud.oscmorph);
    sp_ftbl_destroy(&ud.ft);
    sp_ftbl_destroy(&ud.wt1);
    sp_osc_destroy(&ud.osc);
    
    sp_destroy(&sp);
    return 0;
}
