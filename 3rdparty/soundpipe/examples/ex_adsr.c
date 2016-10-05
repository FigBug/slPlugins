#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "soundpipe.h"

typedef struct {
    sp_adsr *adsr;
    sp_osc *osc;
    sp_ftbl *ft; 
} UserData;

void process(sp_data *sp, void *udata) {
    UserData *ud = udata;
    SPFLOAT osc = 0, adsr = 0, gate = 0;
    if(sp->pos < sp->sr * 3) {
        gate = 1;
    }
    sp_adsr_compute(sp, ud->adsr, &gate, &adsr);
    sp_osc_compute(sp, ud->osc, NULL, &osc);
    sp->out[0] = adsr * osc;
}

int main() {
    srand(1234567);
    UserData ud;
    sp_data *sp;
    sp_create(&sp);

    sp_adsr_create(&ud.adsr);
    sp_osc_create(&ud.osc);
    sp_ftbl_create(sp, &ud.ft, 2048);

    sp_adsr_init(sp, ud.adsr);
    sp_gen_sine(sp, ud.ft);
    sp_osc_init(sp, ud.osc, ud.ft, 0);
    ud.osc->amp = 0.5;

    sp->len = 44100 * 5;
    sp_process(sp, &ud, process);

    sp_adsr_destroy(&ud.adsr);
    sp_ftbl_destroy(&ud.ft);
    sp_osc_destroy(&ud.osc);

    sp_destroy(&sp);
    return 0;
}
