#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "soundpipe.h"

typedef struct {
    sp_drip *drip;
    sp_dust *trig;
    sp_revsc *rev;
} UserData;

void write_osc(sp_data *sp, void *ud) {
    UserData *udp = ud;
    SPFLOAT trig, rev1, rev2, drip;
    sp_dust_compute(sp, udp->trig, NULL, &trig);
    sp_drip_compute(sp, udp->drip, &trig, &drip);
    sp_revsc_compute(sp, udp->rev, &drip, &drip, &rev1, &rev2);
    sp->out[0] = drip + rev1 * 0.05;
}

int main() {
    srand(time(NULL));
    UserData ud;
    sp_data *sp;
    sp_create(&sp);
    sp_revsc_create(&ud.rev);
    sp_drip_create(&ud.drip);
    sp_dust_create(&ud.trig);
    
    sp_dust_init(sp, ud.trig);
    ud.trig->amp = 1;
    ud.trig->density = 1;
    sp_drip_init(sp, ud.drip, 0.09);
    ud.drip->amp = 0.3;
    sp_revsc_init(sp, ud.rev);
    ud.rev->feedback = 0.9;
    
    sp->len = 44100 * 30;
    sp_process(sp, &ud, write_osc);
    sp_drip_destroy(&ud.drip);
    sp_dust_destroy(&ud.trig);
    sp_revsc_destroy(&ud.rev);
    sp_destroy(&sp);
    return 0;
}
