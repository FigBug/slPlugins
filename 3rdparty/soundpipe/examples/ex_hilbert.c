#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "soundpipe.h"

typedef struct {
    sp_hilbert *hilbert;
    sp_osc *cos, *sin;
    sp_ftbl *ft; 
    sp_diskin *diskin;
} UserData;

void process(sp_data *sp, void *udata) {
    UserData *ud = udata;
    SPFLOAT disk= 0;
    SPFLOAT real = 0, imag = 0;
    SPFLOAT diskin = 0;
    SPFLOAT sin = 0, cos = 0;

    sp_diskin_compute(sp, ud->diskin, NULL, &diskin);
    sp_osc_compute(sp, ud->sin, NULL, &sin);
    sp_osc_compute(sp, ud->cos, NULL, &cos);
    sp_hilbert_compute(sp, ud->hilbert, &diskin, &real, &imag);
    sp->out[0] = ((cos * real) + (sin * real)) * 0.7;
}

int main() {
    srand(1234567);
    UserData ud;
    sp_data *sp;
    sp_create(&sp);

    sp_hilbert_create(&ud.hilbert);
    sp_osc_create(&ud.sin);
    sp_osc_create(&ud.cos);
    sp_diskin_create(&ud.diskin);
    sp_ftbl_create(sp, &ud.ft, 8192);

    sp_hilbert_init(sp, ud.hilbert);
    sp_gen_sine(sp, ud.ft);
    sp_osc_init(sp, ud.sin, ud.ft, 0);
    sp_osc_init(sp, ud.cos, ud.ft, 0.25);
    ud.sin->freq = 1000;
    ud.cos->freq = 1000;
    sp_diskin_init(sp, ud.diskin, "oneart.wav");

    sp->len = 44100 * 5;
    sp_process(sp, &ud, process);

    sp_hilbert_destroy(&ud.hilbert);
    sp_ftbl_destroy(&ud.ft);
    sp_osc_destroy(&ud.sin);
    sp_osc_destroy(&ud.cos);
    sp_diskin_destroy(&ud.diskin);

    sp_destroy(&sp);
    return 0;
}
